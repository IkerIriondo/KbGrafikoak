#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "display.h"
#include "io.h"
#include "definitions.h"
#include <math.h>
#include <stdbool.h>

/** GLOBAL VARIABLES **/

GLdouble _window_ratio;                     /*Control of window's proportions */
GLdouble _ortho_x_min,_ortho_x_max;         /*Variables for the control of the orthographic projection*/
GLdouble _ortho_y_min ,_ortho_y_max;        /*Variables for the control of the orthographic projection*/
GLdouble _ortho_z_min,_ortho_z_max;         /*Variables for the control of the orthographic projection*/

object3d * _first_object= 0;                /*List of objects*/
object3d * _selected_object = 0;            /*Object currently selected*/

object3d *_first_kamera = 0;
object3d *_selected_kamera = 0;

argia *bonbila, *eguzkia, *fokoa, *fokoa_obj, *_selected_argia;

char aldaketa_mota; 
char erreferentzi_sistema;
char zer_aldatu;
char kam_mota;
char poligonoak;
char argiak, argi1, argi2, argi3, argi4, argi5, argi6, argi7, argi8;
char flat_smooth;

double ezker,eskuin,behekoa,goikoa,near,far;

/** GENERAL INITIALIZATION **/
void initialization (){
    double mat[16];
    int i;
    object3d *aux_kamera;

    ezker = -0.1;
    eskuin = 0.1;
    behekoa = -0.1;
    goikoa = 0.1;
    near = 0.1;
    far = 1000;
    poligonoak = 'b';
    kam_mota = 'l';
    argiak = 'b';
    argi1 = 'i';
    argi2 = 'i';
    argi3 = 'i';
    argi4 = 'i';
    argi5 = 'i';
    argi6 = 'i';
    argi7 = 'i';
    argi8 = 'i';

    flat_smooth = 'f';

    /*Initialization of all the variables with the default values*/
    _ortho_x_min = KG_ORTHO_X_MIN_INIT;
    _ortho_x_max = KG_ORTHO_X_MAX_INIT;
    _ortho_y_min = KG_ORTHO_Y_MIN_INIT;
    _ortho_y_max = KG_ORTHO_Y_MAX_INIT;
    _ortho_z_min = KG_ORTHO_Z_MIN_INIT;
    _ortho_z_max = KG_ORTHO_Z_MAX_INIT;

    _window_ratio = (GLdouble) KG_WINDOW_WIDTH / (GLdouble) KG_WINDOW_HEIGHT;

    /*Definition of the background color*/
    glClearColor(KG_COL_BACK_R, KG_COL_BACK_G, KG_COL_BACK_B, KG_COL_BACK_A);

    /*Definition of the method to draw the objects*/
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    erreferentzi_sistema = 'g';
    zer_aldatu = 'o';
    aldaketa_mota = 't';

    //(5, 5, 5) posiziotik (0, 0, 0)-ra begira

    aux_kamera = (object3d *)malloc(sizeof(object3d));
    aux_kamera->min.x = ezker;
    aux_kamera->max.x = eskuin;
    aux_kamera->min.y = behekoa;
    aux_kamera->max.y = goikoa;
    aux_kamera->min.z = near;
    aux_kamera->max.z = far;

    mat[0] = sqrt(2)/2;  mat[4] = -sqrt(2)/(2*sqrt(3)); mat[8] = 1/sqrt(3);  mat[12] = 5;
    mat[1] = 0;          mat[5] = sqrt(2)/sqrt(3);      mat[9] = 1/sqrt(3);  mat[13] = 5;
    mat[2] = -sqrt(2)/2; mat[6] = -sqrt(2)/(2*sqrt(3)); mat[10] = 1/sqrt(3); mat[14] = 5;
    mat[3] = 0;          mat[7] = 0;                    mat[11] = 0;         mat[15] = 1;

    aux_kamera->mzptr =(mz *)malloc(sizeof(mz));
    for(i = 0; i<16; i++) aux_kamera->mzptr->matrize[i] = mat[i];
    aux_kamera->next = 0;
    aux_kamera->kam_mota = '0'; //paraleloan, 1 perspektiba

    _first_kamera = aux_kamera;
    _selected_kamera = aux_kamera;

    //Goikoa 

    aux_kamera = (object3d *)malloc(sizeof(object3d));
    aux_kamera->min.x = ezker;
    aux_kamera->max.x = eskuin;
    aux_kamera->min.y = behekoa;
    aux_kamera->max.y = goikoa;
    aux_kamera->min.z = near;
    aux_kamera->max.z = far;

    mat[0] = 1;  mat[4] = 0; mat[8] = 0;  mat[12] = 0;
    mat[1] = 0;  mat[5] = 0; mat[9] = 1;  mat[13] = 5;
    mat[2] = 0;  mat[6] = -1;mat[10] = 0; mat[14] = 0;
    mat[3] = 0;  mat[7] = 0; mat[11] = 0; mat[15] = 1;

    aux_kamera->mzptr =(mz *)malloc(sizeof(mz));
    for(i = 0; i<16; i++) aux_kamera->mzptr->matrize[i] = mat[i];
    aux_kamera->next = _first_kamera;
    aux_kamera->kam_mota = '1'; //paraleloan, 1 perspektiba

    _first_kamera = aux_kamera;
    _selected_kamera = aux_kamera;

    //Albokoa 

    aux_kamera = (object3d *)malloc(sizeof(object3d));
    aux_kamera->min.x = ezker;
    aux_kamera->max.x = eskuin;
    aux_kamera->min.y = behekoa;
    aux_kamera->max.y = goikoa;
    aux_kamera->min.z = near;
    aux_kamera->max.z = far;

    mat[0] = 0;  mat[4] = 0; mat[8] = 1;  mat[12] = 5;
    mat[1] = 0;  mat[5] = 1; mat[9] = 0;  mat[13] = 0;
    mat[2] = -1; mat[6] = 0; mat[10] = 0; mat[14] = 0;
    mat[3] = 0;  mat[7] = 0; mat[11] = 0; mat[15] = 1;

    aux_kamera->mzptr =(mz *)malloc(sizeof(mz));
    for(i = 0; i<16; i++) aux_kamera->mzptr->matrize[i] = mat[i];
    aux_kamera->next = _first_kamera;
    aux_kamera->kam_mota = '0'; //paraleloan, 1 perspektiba

    _first_kamera = aux_kamera;
    _selected_kamera = aux_kamera;

    printf("Erreferentzi sistema: %c, Zer aldatu: %c, Aldaketa mota: %c\n", erreferentzi_sistema, zer_aldatu, aldaketa_mota);

    //ARGIAK
    _selected_argia = 0;
    GLfloat horia [4] = {0.0 , 1.0 , 1.0 , 1.0};
    GLfloat grisa [4] = {0.2 , 0.2 , 0.2 , 1.0};
    GLfloat txuria [4] = {1.0 , 1.0 , 1.0 , 1.0};

    //BONBILA

    GLfloat lag[4];
    lag[0] = 0.0; lag[1] = 10.0; lag[2] = 0.0; lag[3] = 1.0;

    bonbila = (argia *)malloc(sizeof(argia));
    bonbila->argi_zenb = GL_LIGHT0;
    bonbila->kokapena = (bekz *)malloc(sizeof(bekz));
    for(i = 0; i<4; i++) bonbila->kokapena->bektorea[i] = lag[i];
    bonbila->kokapena2 = (bekz *)malloc(sizeof(bekz));
    bonbila->angelua = 180.0;
    bonbila->argi_mota = 'b';
    bonbila->const_at = 1.0;
    bonbila->lin_at = 0.2;
    bonbila->quad_at = 0.0;

    glLightf(bonbila->argi_zenb, GL_CONSTANT_ATTENUATION, bonbila->const_at);
    glLightf(bonbila->argi_zenb, GL_LINEAR_ATTENUATION, bonbila->lin_at);
    glLightf(bonbila->argi_zenb, GL_QUADRATIC_ATTENUATION, bonbila->quad_at);

    glLightfv(bonbila->argi_zenb, GL_AMBIENT, grisa);
    glLightfv(bonbila->argi_zenb, GL_DIFFUSE, horia);
    glLightfv(bonbila->argi_zenb, GL_SPECULAR, txuria);

    glLightfv(bonbila->argi_zenb, GL_POSITION, bonbila->kokapena->bektorea);
    glLightf(bonbila->argi_zenb, GL_SPOT_CUTOFF, bonbila->angelua);

    //EGUZKIA

    //GLfloat lag[4];
    lag[0] = 1.0; lag[1] = 0.0; lag[2] = 0.0; lag[3] = 0.0;

    eguzkia = (argia *)malloc(sizeof(argia));
    eguzkia->argi_zenb = GL_LIGHT1;
    eguzkia->norabidea = (bekz *)malloc(sizeof(bekz));
    for(i = 0; i<4; i++) eguzkia->norabidea->bektorea[i] = lag[i];
    eguzkia->norabidea2 = (bekz *)malloc(sizeof(bekz));
    eguzkia->argi_mota = 'e';
    eguzkia->const_at = 1.0;
    eguzkia->lin_at = 0.2;
    eguzkia->quad_at = 0.0;

    glLightf(eguzkia->argi_zenb, GL_CONSTANT_ATTENUATION, eguzkia->const_at);
    glLightf(eguzkia->argi_zenb, GL_LINEAR_ATTENUATION, eguzkia->lin_at);
    glLightf(eguzkia->argi_zenb, GL_QUADRATIC_ATTENUATION, eguzkia->quad_at);

    glLightfv(eguzkia->argi_zenb, GL_AMBIENT, grisa);
    glLightfv(eguzkia->argi_zenb, GL_DIFFUSE, horia);
    glLightfv(eguzkia->argi_zenb, GL_SPECULAR, txuria);

    glLightfv(eguzkia->argi_zenb, GL_POSITION, eguzkia->norabidea->bektorea);

    //FOKOA (objektua)

    fokoa_obj = (argia *)malloc(sizeof(argia));

    fokoa_obj->argi_zenb = GL_LIGHT2;

    fokoa_obj->angelua = 45.0;
    fokoa_obj->intentsitatea = 0.7;
    fokoa_obj->argi_mota = 'f';
    fokoa_obj->const_at = 1.0;
    fokoa_obj->lin_at = 0.2;
    fokoa_obj->quad_at = 0.0;

    glLightf(fokoa_obj->argi_zenb, GL_CONSTANT_ATTENUATION, fokoa_obj->const_at);
    glLightf(fokoa_obj->argi_zenb, GL_LINEAR_ATTENUATION, fokoa_obj->lin_at);
    glLightf(fokoa_obj->argi_zenb, GL_QUADRATIC_ATTENUATION, fokoa_obj->quad_at);

    glLightfv(fokoa_obj->argi_zenb, GL_AMBIENT, grisa);
    glLightfv(fokoa_obj->argi_zenb, GL_DIFFUSE, horia);
    glLightfv(fokoa_obj->argi_zenb, GL_SPECULAR, txuria);


    //FOKOA (kamera)

    GLfloat lag1[4];
    GLfloat lag2[4];
    lag1[0] = _selected_kamera->mzptr->matrize[12];
    lag1[1] = _selected_kamera->mzptr->matrize[13];
    lag1[2] = _selected_kamera->mzptr->matrize[14];
    lag1[3] = _selected_kamera->mzptr->matrize[15];

    lag2[0] = -_selected_kamera->mzptr->matrize[8];
    lag2[1] = -_selected_kamera->mzptr->matrize[9];
    lag2[2] = -_selected_kamera->mzptr->matrize[10];


    fokoa = (argia *)malloc(sizeof(argia));
    fokoa->argi_zenb = GL_LIGHT3;
    fokoa->kokapena = (bekz *)malloc(sizeof(bekz));
    for(i = 0; i<4; i++) fokoa->kokapena->bektorea[i] = lag1[i];
    fokoa->norabidea = (bekz *)malloc(sizeof(bekz));
    for(i = 0; i<4; i++) fokoa->norabidea->bektorea[i] = lag2[i];
    fokoa->angelua = 45.0;
    fokoa->intentsitatea = 0.7;
    fokoa->argi_mota = 'f';
    fokoa->const_at = 1.0;
    fokoa->lin_at = 0.2;
    fokoa->quad_at = 0.0;

    glLightf(fokoa->argi_zenb, GL_CONSTANT_ATTENUATION, fokoa->const_at);
    glLightf(fokoa->argi_zenb, GL_LINEAR_ATTENUATION, fokoa->lin_at);
    glLightf(fokoa->argi_zenb, GL_QUADRATIC_ATTENUATION, fokoa->quad_at);

    glLightfv(fokoa->argi_zenb, GL_AMBIENT, grisa);
    glLightfv(fokoa->argi_zenb, GL_DIFFUSE, horia);
    glLightfv(fokoa->argi_zenb, GL_SPECULAR, txuria);

    glLightfv(fokoa->argi_zenb, GL_POSITION, fokoa->kokapena->bektorea);
    glLightfv(fokoa->argi_zenb, GL_SPOT_DIRECTION, fokoa->norabidea->bektorea);
    glLightf(fokoa->argi_zenb, GL_SPOT_CUTOFF , fokoa->angelua);
    glLightf(fokoa->argi_zenb, GL_SPOT_EXPONENT, fokoa->intentsitatea);

}


/** MAIN FUNCTION **/
int main(int argc, char** argv) {

    /*First of all, print the help information*/
    print_help();

    /* glut initializations */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(KG_WINDOW_WIDTH, KG_WINDOW_HEIGHT);
    glutInitWindowPosition(KG_WINDOW_X, KG_WINDOW_Y);
    glutCreateWindow(KG_WINDOW_TITLE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);


    /* set the callback functions */
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutSpecialFunc(tekla_berezien_arretarako_funtzioa);

    /* this initialization has to be AFTER the creation of the window */
    initialization();

    /* start the main loop */
    glutMainLoop();
    return 0;
}
