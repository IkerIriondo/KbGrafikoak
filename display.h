#ifndef DISPLAY_H
#define DISPLAY_H

void display(void);
void reshape(int width, int height);
void drawText(const char * text, int length, int x, int y);
void kam_objri_begira();
void kamera_esferan_biratu(double *v);
void posizio_berriak_lortu(double *esam, GLfloat *v);
void esam_matrizea_lortu(double *esamptr, double *mptr);

#endif // DISPLAY_H
