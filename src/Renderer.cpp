#include "../include/Renderer.h"
#include <iostream>

void Renderer::draw() {
    for (int j{}; j < Renderer::m_height; ++j) {
        for(int i{}; i < Renderer::m_width; ++i) {
            std::cout << '#';
        }
        std::cout << '\n';
    }
}

void Renderer::framebuffer() {
    
}