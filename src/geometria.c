#include "geometria.h"
#include "linha.h"

#define EPSILON 1e-10
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


double calcular_distancia_ponto_segmento(ponto *p, anteparo *a) {
    double px = get_x_ponto(p);
    double py = get_y_ponto(p);
    double x1 = get_x_ponto(get_p0_anteparo(a));
    double y1 = get_y_ponto(get_p0_anteparo(a));
    double x2 = get_x_ponto(get_p1_anteparo(a));
    double y2 = get_y_ponto(get_p1_anteparo(a));

    double dx = x2 - x1;
    double dy = y2 - y1;

    double len_sq = dx * dx + dy * dy;
    if (fabs(len_sq) < EPSILON) {
        return sqrt(distancia_quadrada(px, py, x1, y1));
    }

    double t = ((px - x1) * dx + (py - y1) * dy) / len_sq;

    t = fmax(0.0, fmin(1.0, t));

    double proj_x = x1 + t * dx;
    double proj_y = y1 + t * dy;

    return sqrt(distancia_quadrada(px, py, proj_x, proj_y));
}
