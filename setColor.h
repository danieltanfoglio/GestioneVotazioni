#ifndef COLORX_H
#define COLORX_H

#include <iostream>
#include <string>

enum Colore {
    // Colori Standard
    Rosso, Verde, Giallo, Blu, Magenta, Cyan, Bianco,
    // Colori Chiari (Bright)
    RossoChiaro, VerdeChiaro, GialloChiaro, BluChiaro, MagentaChiaro, CyanChiaro, BiancoSplendente,
    // Reset
    Reset
};

inline void setColor(Colore c) {
    switch (c) {
        // Standard (30-37)
        case Rosso:          std::cout << "\033[31m"; break;
        case Verde:          std::cout << "\033[32m"; break;
        case Giallo:         std::cout << "\033[33m"; break;
        case Blu:            std::cout << "\033[34m"; break;
        case Magenta:        std::cout << "\033[35m"; break;
        case Cyan:           std::cout << "\033[36m"; break;
        case Bianco:         std::cout << "\033[37m"; break;
        
        // Bright (90-97)
        case RossoChiaro:    std::cout << "\033[91m"; break;
        case VerdeChiaro:    std::cout << "\033[92m"; break;
        case GialloChiaro:   std::cout << "\033[93m"; break;
        case BluChiaro:      std::cout << "\033[94m"; break;
        case MagentaChiaro:  std::cout << "\033[95m"; break;
        case CyanChiaro:     std::cout << "\033[96m"; break;
        case BiancoSplendente: std::cout << "\033[97m"; break;
        
        case Reset:          std::cout << "\033[0m";  break;
    }
}

#endif
