#ifndef IO_H
#define IO_H

void eskuinetik_biderkatu(double *Mberria, double *Mald, double *Mobj);
void ezkerretik_biderkatu(double *Mberria, double *Mald, double *Mobj);
void aldatu_obj(double *Mberria, double *Mald);

void keyboard(unsigned char key, int x, int y);
void print_help();
void tekla_berezien_arretarako_funtzioa(int key, int x, int y);

#endif // IO_H
