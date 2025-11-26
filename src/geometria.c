#include "geometria.h"

#include <float.h>

#include "linha.h"
#include <math.h>
#include <stddef.h>

#define EPSILON 1e-10
#define PI 3.14159265358

double calcula_angulo(ponto *obs, ponto *principal) {
    double obs_x = get_x_ponto(obs);
    double obs_y = get_y_ponto(obs);
    double p_x = get_x_ponto(principal);
    double p_y = get_y_ponto(principal);

    double angulo = atan2(p_y - obs_y, p_x - obs_x);
    if (angulo < 0) {
        angulo += 2.0 * PI;
    }

    return angulo;
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

double calc_dist_anteparo_bomba(anteparo *a, ponto *p_bomba, double angulo, double raio_max) {
    if (a == NULL || p_bomba == NULL) return DBL_MAX;

    double bx = get_x_ponto(p_bomba);
    double by = get_y_ponto(p_bomba);

    double rx = bx + raio_max * cos(angulo);
    double ry = by + raio_max * sin(angulo);

    ponto *p0 = get_p0_anteparo(a);
    ponto *p1 = get_p1_anteparo(a);
    if (p0 == NULL || p1 == NULL) return DBL_MAX;

    double s1x = get_x_ponto(p0);
    double s1y = get_y_ponto(p0);
    double s2x = get_x_ponto(p1);
    double s2y = get_y_ponto(p1);

    double denom = (s2y - s1y) * (rx - bx) - (s2x - s1x) * (ry - by);

    if (fabs(denom) < EPSILON) return DBL_MAX;

    double ua = ((s2x - s1x) * (by - s1y) - (s2y - s1y) * (bx - s1x)) / denom;
    double ub = ((rx - bx) * (by - s1y) - (ry - by) * (bx - s1x)) / denom;

    if (ua >= -EPSILON && ua <= 1.0 + EPSILON && ub >= -EPSILON && ub <= 1.0 + EPSILON) {
        double dist = ua * raio_max;

        if (dist >= -EPSILON && dist <= raio_max + EPSILON) {
            return (dist < 0) ? 0 : dist;
        }
    }
    return DBL_MAX;
}

double dist_pontos(ponto *a, ponto *b) {
    double x1 = get_x_ponto(a); double x2 = get_x_ponto(b);
    double y1 = get_y_ponto(a); double y2 = get_y_ponto(b);

    return distancia_quadrada(x1, y1, x2, y2);
}
