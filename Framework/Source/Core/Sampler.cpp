#include "Core/Sampler.h"

namespace HLF
{
/*
    static const double CALIBER		= 0.9;
    static const int DRAFT_SIZE		= 48;
    static const double AMPLITUDE	= 1.05;

    Puppet m_hand;
    Engine m_engine;
    Vector3d m_neutral;
*/

Sampler::Sampler(Puppet p)
: m_hand(p), m_engine(45.0), m_neutral(0, 0, 0) {}

MatrixXi Sampler::createRaw(int wh, bool skeleton, double dist) {

    MatrixXi sample;
    m_engine.setPosition(.05 * dist, .05 * dist, dist);
    m_engine.wireFrame(m_hand, wh, wh);

    if (skeleton) {
        sample = 128 * m_engine.getScreen();
        m_engine.fleshHull(CALIBER, 0);
        sample += 127 * m_engine.getScreen();
    } else {
        m_engine.fleshHull(CALIBER, 0);
        sample = 255 * m_engine.getScreen();
    }
    
    return sample;
}

MatrixXi Sampler::createAdjusted(int wh, bool skeleton, double dist) {

	// Evaluate the sample geometry on a draft sample
	MatrixXi draft = createRaw(DRAFT_SIZE, false, dist);

    Vector2d center(0,0);
    VectorXd stair = VectorXd::LinSpaced(DRAFT_SIZE, 1, DRAFT_SIZE);

    for (int i = 0; i < DRAFT_SIZE; i++){
        center(0) += (draft.col(i).cast <double> ()).dot(stair);
        center(1) += (draft.row(i).cast <double> ()).dot(stair);
    }
    
    center /= draft.sum();

    double upper_sqd = 0;
    for(int i = 0; i < DRAFT_SIZE; i++)
        for(int j = 0; j < DRAFT_SIZE; j++)
            if (draft(i,j) != 0) {
                upper_sqd = std::max(upper_sqd, (center - Vector2d(i, j)).squaredNorm());
            }
    
    double radius = std::sqrt(upper_sqd) * AMPLITUDE;
    double scaling = DRAFT_SIZE / (2 * radius);
    
    scaling *= 2; // super-sampling to cleanse the aliasing
    
    MatrixXi sample = createRaw(wh * scaling, false, dist);
    
    center *= (scaling * wh / DRAFT_SIZE);
    radius *= (scaling * wh / DRAFT_SIZE);
    
    Vector2d corner(std::floor(center(0) - radius), std::floor(center(1) - radius));

    MatrixXi cropped = sample.block(corner(0), corner(1), 2 * radius, 2 * radius);
    
    bilinear(cropped, wh);

    return cropped;
}

double Sampler::scale(double x, double a, double b) {
    return x * (b - a) + a;
}

// Neutral (anchor) rotation setter
void Sampler::setNeutral(Vector3d neutral) {
    m_neutral = neutral;
}

// DoF magnitude setter
void Sampler::setDoF(int id, double dr) {
    switch (id) {
        case 0: {
            m_hand.setHinge( 0, scale( dr, -0.1, 0.4));
            break;
        }
        
        case 1: {
            m_hand.setHinge( 1, scale( dr,-0.6, 0.7));
            m_hand.setHinge( 2, scale( dr,-0.3, 1.2));
            break;
        }
        
        case 2: {
            m_hand.setHinge( 3, scale( dr,-0.1, 1.3));
            m_hand.setHinge( 4, scale( dr, 0.0, 1.9));
            m_hand.setHinge( 5, scale( dr, 0.0, 1.6));
            break;
        }
        
        case 3: {
            m_hand.setHinge( 6, scale( dr,-0.1, 1.2));
            m_hand.setHinge( 7, scale( dr, 0.0, 1.9));
            m_hand.setHinge( 8, scale( dr, 0.0, 1.6));
            break;
        }
        
        case 4: {
            m_hand.setHinge( 9, scale( dr,-0.1, 1.3));
            m_hand.setHinge(10, scale( dr, 0.0, 1.9));
            m_hand.setHinge(11, scale( dr, 0.0, 1.6));
            break;
        }
        
        case 5: {
            m_hand.setHinge(12, scale( dr,-0.1, 1.3));
            m_hand.setHinge(13, scale( dr, 0.0, 1.9));
            m_hand.setHinge(14, scale( dr, 0.0, 1.6));
            break;
        }
        
        case 6: {
            m_hand.setHinge(15, scale( dr,-0.1, 0.2));
            m_hand.setHinge(16, scale( dr, 0.0, 0.0));
            m_hand.setHinge(17, scale( dr,-0.1, 0.2));
            m_hand.setHinge(18, scale( dr,-0.2, 0.6));
            break;
        }
        
        case 7: {
            m_engine.setRotX(m_neutral(0) + scale(dr, 0.0, -1.0));
            break;
        }
        
        case 8: {
            m_engine.setRotY(m_neutral(1) + scale(dr,0.0, -1.5));
            break;
        }
        
        case 9: {
            m_engine.setRotZ(m_neutral(2) + scale(dr,0.5, -0.9));
            break;
        }
    }
}

void Sampler::bilinear(MatrixXi& input, int wh_dest) {

    int wh_src = input.rows();
    MatrixXi output = MatrixXi::Zero(wh_dest, wh_dest);

    int A, B, C, D, x, y;
    double magnitude = ((double) (wh_src - 1)) / wh_dest;
    double x_diff, y_diff;

    for (int i = 0; i < wh_dest; i++) {
        for (int j = 0; j < wh_dest; j++) {
        
            x = (int) (magnitude * i);
            y = (int) (magnitude * j);

            x_diff = (magnitude * i) - x;
            y_diff = (magnitude * j) - y;

            A = input(    x,     y);
            B = input(x + 1,     y);
            C = input(    x, y + 1);
            D = input(x + 1, y + 1);;

            // Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
            output(i, j) = (int) (A * (1 - x_diff) * (1 - y_diff)
                                + B * (x_diff) * (1 - y_diff)
                                + C * (y_diff) * (1 - x_diff)
                                + D * (x_diff * y_diff));
        }
    }
    input = output;
}
}
