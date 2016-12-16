#include "Core/Puppet.h"

namespace HLF
{
Puppet::Puppet(const char * path) { 
	load(path); 
}

void Puppet::load(const char * path) {

    std::ifstream file(path);
    if (file == NULL) {
        std::cout << "Failed to open the mesh \"" << path << "\"\n";
        exit(0);
    }

    std::string line, head;

    while(!file.eof()) {
    
        std::getline(file, line);
        std::stringstream strstr(line);
        std::getline(strstr, head, ' ');

        if (strcmp(head.c_str(), "v") == 0) {
        
            Vector3d vertice;
            strstr >> vertice(0) >> vertice(1) >> vertice(2);
            m_vertices.push_back(vertice);
            
        } else if (strcmp(head.c_str(), "e") == 0) {
        
            Vector2i edge;
            strstr >> edge(0) >> edge(1);
            m_edges.push_back(edge);
            
        } else if (strcmp(head.c_str(), "vt") == 0) {
        
            Vector3i triangle;
            strstr >> triangle(0) >> triangle(1) >> triangle(2);
            m_triangles.push_back(triangle);
            
        } else if (strcmp(head.c_str(), "h") == 0) {
                
            Hinge hinge;
            hinge.magnitude = 0.f;
            
            strstr >> hinge.pivot;
            strstr >> hinge.axis(0) >> hinge.axis(1) >> hinge.axis(2);

            int index;
            while (strstr >> index)
                hinge.id.push_back(index);

            m_hinges.push_back(hinge);
        }
    }
}

// Rewrite this thing with printf (?)
void Puppet::printContent() {

    std::cout << "--- Vertices ---" << std::endl;
    for(int i = 0; i < (int) m_vertices.size(); i++) {
            std::cout << std::setw(5) << m_vertices[i](0)
                      << std::setw(5) << m_vertices[i](1)
                      << std::setw(5) << m_vertices[i](2)
                      << std::endl;
    }

    std::cout << "----- Edges -----" << std::endl;
    for(int i = 0; i < (int) m_edges.size(); i++) {
        std::cout << "("   << std::setw(4) << m_edges[i](0)
                  << " ->" << std::setw(4) << m_edges[i](1)
                  << ")"   << std::endl;
    }

    std::cout << "--- Triangles ---" << std::endl;
    for(int i = 0; i < (int) m_triangles.size(); i++) {
        std::cout << "[" << std::setw(3) << m_triangles[i](0)
                  << " " << std::setw(3) << m_triangles[i](1)
                  << " " << std::setw(3) << m_triangles[i](2)
                  << "]" << std::endl;
    }

    std::cout << "---- Hinges ----" << std::endl;
    for(int i = 0; i < (int) m_hinges.size(); i++) {
           
        std::cout << "pivot: " << std::setw(4) << m_hinges[i].pivot
                  << ";  a: [" << std::setw(4) << m_hinges[i].axis.transpose()
                  << " ];\t i: ( ";

        for(int j = 0; j < (int) m_hinges[i].id.size(); j++)
            std::cout << m_hinges[i].id[j] << " ";
            
        std::cout << ")" << std::endl;
    }
}

// TODO: use a table to access the leafs
void Puppet::setHinge(int hi, double magnitude) {

    int pivot = m_hinges[hi].pivot;
    double delta = magnitude - m_hinges[hi].magnitude;

    Matrix3d R = AngleAxisd(delta, m_hinges[hi].axis.normalized()).toRotationMatrix();
    m_hinges[hi].magnitude = magnitude;

    for (int j = 0; j < (int) m_hinges[hi].id.size(); j++) {
        int leaf = m_hinges[hi].id[j];

        m_vertices[leaf] = R * (m_vertices[leaf] - m_vertices[pivot]) + m_vertices[pivot];

        for (int k = 0; k < (int) m_hinges.size(); k++) {
            if (m_hinges[k].pivot == leaf) {
                m_hinges[k].axis = R * m_hinges[k].axis;
                break;
            }
        }
    }
}

int Puppet::nbVert() { 
	return (int) m_vertices.size(); 
}

int Puppet::nbEdge() { 
	return (int) m_edges.size(); 
}

int Puppet::nbTril() { 
	return (int) m_triangles.size(); 
}

Vector3d Puppet::getVertex(int vi) { 
	return m_vertices[vi]; 
}

Vector2i Puppet::getEdge(int ei) { 
	return m_edges[ei]; 
}

Vector3i Puppet::getTriangle(int ti) { 
	return m_triangles[ti]; 
}
}

