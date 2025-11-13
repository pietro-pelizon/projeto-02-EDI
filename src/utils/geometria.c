#include "geometria.h"
#include "linha.h"

#define PI 3.14159265358

double calcula_angulo(ponto *obs, ponto *principal) {
    double obs_x = get_x_ponto(obs);
    double obs_y = get_y_ponto(obs);
    double p_x = get_x_ponto(principal);
    double p_y = get_y_ponto(principal);

    return atan2(p_y - obs_y, p_x - obs_x);
}

double distancia_quadrada(double x1, double y1, double x2, double y2) {
    double deltaX = x1 - x2;
    double deltaY = y1 - y2;
    return (deltaX * deltaX) + (deltaY * deltaY);
}

int produto_vetorial(double px, double py, double qx, double qy, double rx, double ry) {
    double val = (qx - px) * (ry - py) - (qy - py) * (rx - px);
    if (fabs(val) < 1e-10) return 0;
    return (val > 0) ? 1 : 2;
}

bool is_ponto_no_segmento(double px, double py, double qx, double qy, double rx, double ry) {
    return (qx <= fmax(px, rx) && qx >= fmin(px, rx) && qy <= fmax(py, ry) && qy >= fmin(py, ry));
}

