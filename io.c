#include "definitions.h"
#include "load_obj.h"
#include "display.h"
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

extern object3d * _first_object;
extern object3d * _selected_object;

extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

extern char aldaketa_mota;
extern char erreferentzi_sistema;
extern char zer_aldatu;

/**
 * @brief This function just prints information about the use
 * of the keys
 */
void print_help(){
    printf("KbG Irakasgaiaren Praktika. Programa honek 3D objektuak \n");
    printf("aldatzen eta bistaratzen ditu.  \n\n");
    printf("\n\n");
    printf("FUNTZIO NAGUSIAK \n");
    printf("<?>\t\t Laguntza hau bistaratu \n");
    printf("<ESC>\t\t Programatik irten \n");
    printf("<F>\t\t Objektua bat kargatu\n");
    printf("<TAB>\t\t Kargaturiko objektuen artean bat hautatu\n");
    printf("<DEL>\t\t Hautatutako objektua ezabatu\n");
    printf("<CTRL + ->\t Bistaratze-eremua handitu\n");
    printf("<CTRL + +>\t Bistaratze-eremua txikitu\n");
    printf("<I>\t\t Objektuaren informazioa bistaratu\n");
    printf("<T>\t\t Traslazioa aktibatu\n");
    printf("<R>\t\t Biraketa aktibatu\n");
    printf("<E>\t\t Eskalaketa aktibatu\n");
    printf("<G>\t\t Aldaketak munduaren erreferentzi sisteman\n");
    printf("<L>\t\t Aldaketak lokalak\n");
    printf("<O>\t\t Aldaketak objektuen gainean\n");
    printf("<C>\t\t Aldaketak kamerari\n");
    printf("<L>\t\t Aldaketak argiei\n");
    printf("<Z>\t\t Aldaketak desegin\n");
    printf("<GORA>\t\t Mugitu +Y | Txikitu Y | Biratu -X\n");
    printf("<BEHERA>\t Mugitu -Y | Handitu Y | Biratu +X\n");
    printf("<ESKUIN>\t Mugitu +X | Txikitu X | Biratu +Y\n");
    printf("<EZKER>\t\t Mugitu -X | Handitu X | Biratu -Y\n");
    printf("<AVPAG>\t\t Mugitu -Z | Txikitu Z | Biratu +Z\n");
    printf("<REPAG>\t\t Mugitu +Z | Handitu Z | Biratu -Z\n");
    printf("\n\n");
}

/*objektu baten barne memoria liberatzen duen funtzioa*/
void free_object(object3d *obptr){
    int i;
    mz *auxptr;
    for(i = 0; i<obptr->num_faces; i++) free(obptr->face_table[i].vertex_table);
    free(obptr->vertex_table);
    free(obptr->face_table);
    /*TODO Matrizeekin zer gertatzen da?*/
    for(auxptr = obptr->mzptr; auxptr != 0; auxptr = obptr->mzptr){
        obptr->mzptr = obptr->mzptr->next;
        free(auxptr);
    }
}

void lortu_traslazio_matrizea(double *mptr, double x, double y, double z){
    
    mptr[0] = 1; mptr[4] = 0; mptr[8] = 0;  mptr[12] = x;
    mptr[1] = 0; mptr[5] = 1; mptr[9] = 0;  mptr[13] = y;
    mptr[2] = 0; mptr[6] = 0; mptr[10] = 1; mptr[14] = z;
    mptr[3] = 0; mptr[7] = 0; mptr[11] = 0; mptr[15] = 1;
    
}

void lortu_eskalatu_matrizea(double *mptr, double p, double q, double r){
   
    mptr[0] = p; mptr[4] = 0; mptr[8] = 0;  mptr[12] = 0;
    mptr[1] = 0; mptr[5] = q; mptr[9] = 0;  mptr[13] = 0;
    mptr[2] = 0; mptr[6] = 0; mptr[10] = r; mptr[14] = 0;
    mptr[3] = 0; mptr[7] = 0; mptr[11] = 0; mptr[15] = 1;
    
}

void lortu_biratu_matrizea(double *mptr, double x, double y, double z){

    double theta;
    theta = 0.3;

    mptr[0] = cos(theta) + x*x*(1.0-cos(theta));     mptr[4] = x*y*(1.0 - cos(theta)) - z*sin(theta);  mptr[8] = x*z*(1.0 - cos(theta)) + y*sin(theta);  mptr[12] = 0;
    mptr[1] = x*y*(1.0 - cos(theta)) + z*sin(theta); mptr[5] = cos(theta) + y*y*(1.0-cos(theta));      mptr[9] = y*z*(1.0 - cos(theta)) - x*sin(theta);  mptr[13] = 0;
    mptr[2] = x*z*(1.0 - cos(theta)) - y*sin(theta); mptr[6] = y*z*(1.0 - cos(theta)) + x*sin(theta);  mptr[10] = cos(theta) + z*z*(1-cos(theta));       mptr[14] = 0;
    mptr[3] = 0;                                     mptr[7] = 0;                                      mptr[11] = 0;                                     mptr[15] = 1;

}

void eskuinetik_biderkatu(double *Mberria, double *Mald, double *Mobj){

    Mberria[0] =  Mobj[0]*Mald[0] + Mobj[1]*Mald[4] + Mobj[2]*Mald[8] + Mobj[3]*Mald[12]; 
    Mberria[1] =  Mobj[0]*Mald[1] + Mobj[1]*Mald[5] + Mobj[2]*Mald[9] + Mobj[3]*Mald[13]; 
    Mberria[2] =  Mobj[0]*Mald[2] + Mobj[1]*Mald[6] + Mobj[2]*Mald[10] + Mobj[3]*Mald[14]; 
    Mberria[3] =  Mobj[0]*Mald[3] + Mobj[1]*Mald[7] + Mobj[2]*Mald[11] + Mobj[3]*Mald[15]; 

    Mberria[4] =  Mobj[4]*Mald[0] + Mobj[5]*Mald[4] + Mobj[6]*Mald[8] + Mobj[7]*Mald[12]; 
    Mberria[5] =  Mobj[4]*Mald[1] + Mobj[5]*Mald[5] + Mobj[6]*Mald[9] + Mobj[7]*Mald[13];
    Mberria[6] =  Mobj[4]*Mald[2] + Mobj[5]*Mald[6] + Mobj[6]*Mald[10] + Mobj[7]*Mald[14]; 
    Mberria[7] =  Mobj[4]*Mald[3] + Mobj[5]*Mald[7] + Mobj[6]*Mald[11] + Mobj[7]*Mald[15]; 

    Mberria[8] =  Mobj[8]*Mald[0] + Mobj[9]*Mald[4] + Mobj[10]*Mald[8] + Mobj[11]*Mald[12]; 
    Mberria[9] =  Mobj[8]*Mald[1] + Mobj[9]*Mald[5] + Mobj[10]*Mald[9] + Mobj[11]*Mald[13];
    Mberria[10] =  Mobj[8]*Mald[2] + Mobj[9]*Mald[6] + Mobj[10]*Mald[10] + Mobj[11]*Mald[14]; 
    Mberria[11] =  Mobj[8]*Mald[3] + Mobj[9]*Mald[7] + Mobj[10]*Mald[11] + Mobj[11]*Mald[15]; 

    Mberria[12] =  Mobj[12]*Mald[0] + Mobj[13]*Mald[4] + Mobj[14]*Mald[8] + Mobj[15]*Mald[12]; 
    Mberria[13] =  Mobj[12]*Mald[1] + Mobj[13]*Mald[5] + Mobj[14]*Mald[9] + Mobj[15]*Mald[13];
    Mberria[14] =  Mobj[12]*Mald[2] + Mobj[13]*Mald[6] + Mobj[14]*Mald[10] + Mobj[15]*Mald[14]; 
    Mberria[15] =  Mobj[12]*Mald[3] + Mobj[13]*Mald[7] + Mobj[14]*Mald[11] + Mobj[15]*Mald[15];

}

void ezkerretik_biderkatu(double *Mberria, double *Mald, double *Mobj){

    Mberria[0] =  Mald[0]*Mobj[0] + Mald[1]*Mobj[4] + Mald[2]*Mobj[8] + Mald[3]*Mobj[12]; 
    Mberria[1] =  Mald[0]*Mobj[1] + Mald[1]*Mobj[5] + Mald[2]*Mobj[9] + Mald[3]*Mobj[13]; 
    Mberria[2] =  Mald[0]*Mobj[2] + Mald[1]*Mobj[6] + Mald[2]*Mobj[10] + Mald[3]*Mobj[14]; 
    Mberria[3] =  Mald[0]*Mobj[3] + Mald[1]*Mobj[7] + Mald[2]*Mobj[11] + Mald[3]*Mobj[15]; 

    Mberria[4] =  Mald[4]*Mobj[0] + Mald[5]*Mobj[4] + Mald[6]*Mobj[8] + Mald[7]*Mobj[12]; 
    Mberria[5] =  Mald[4]*Mobj[1] + Mald[5]*Mobj[5] + Mald[6]*Mobj[9] + Mald[7]*Mobj[13];
    Mberria[6] =  Mald[4]*Mobj[2] + Mald[5]*Mobj[6] + Mald[6]*Mobj[10] + Mald[7]*Mobj[14]; 
    Mberria[7] =  Mald[4]*Mobj[3] + Mald[5]*Mobj[7] + Mald[6]*Mobj[11] + Mald[7]*Mobj[15]; 

    Mberria[8] =  Mald[8]*Mobj[0] + Mald[9]*Mobj[4] + Mald[10]*Mobj[8] + Mald[11]*Mobj[12]; 
    Mberria[9] =  Mald[8]*Mobj[1] + Mald[9]*Mobj[5] + Mald[10]*Mobj[9] + Mald[11]*Mobj[13];
    Mberria[10] =  Mald[8]*Mobj[2] + Mald[9]*Mobj[6] + Mald[10]*Mobj[10] + Mald[11]*Mobj[14]; 
    Mberria[11] =  Mald[8]*Mobj[3] + Mald[9]*Mobj[7] + Mald[10]*Mobj[11] + Mald[11]*Mobj[15]; 

    Mberria[12] =  Mald[12]*Mobj[0] + Mald[13]*Mobj[4] + Mald[14]*Mobj[8] + Mald[15]*Mobj[12]; 
    Mberria[13] =  Mald[12]*Mobj[1] + Mald[13]*Mobj[5] + Mald[14]*Mobj[9] + Mald[15]*Mobj[13];
    Mberria[14] =  Mald[12]*Mobj[2] + Mald[13]*Mobj[6] + Mald[14]*Mobj[10] + Mald[15]*Mobj[14]; 
    Mberria[15] =  Mald[12]*Mobj[3] + Mald[13]*Mobj[7] + Mald[14]*Mobj[11] + Mald[15]*Mobj[15];

}

/**
 * @brief Callback function to control the basic keys
 * @param key Key that has been pressed
 * @param x X coordinate of the mouse pointer when the key was pressed
 * @param y Y coordinate of the mouse pointer when the key was pressed
 */
void keyboard(unsigned char key, int x, int y) {

    char fname[128]; /* Note that scanf adds a null character at the end of the vector*/
    int read = 0;
    object3d *auxiliar_object = 0;
    GLdouble wd,he,midx,midy;

    mz *mzptr2;

    switch (key) {
    case 'f':
    case 'F':
        /*Ask for file*/
        printf("%s", KG_MSSG_SELECT_FILE);
        scanf("%s", &(fname[0]));
        /*Allocate memory for the structure and read the file*/
        auxiliar_object = (object3d *) malloc(sizeof (object3d));
        read = read_wavefront(fname, auxiliar_object);
        switch (read) {
        /*Errors in the reading*/
        case 1:
            printf("%s: %s\n", fname, KG_MSSG_FILENOTFOUND);
            break;
        case 2:
            printf("%s: %s\n", fname, KG_MSSG_INVALIDFILE);
            break;
        case 3:
            printf("%s: %s\n", fname, KG_MSSG_EMPTYFILE);
            break;
        /*Read OK*/
        case 0:
            /*Insert the new object in the list*/
            auxiliar_object->mzptr = (mz *)malloc(sizeof(mz));
            auxiliar_object->mzptr->next = 0;
            for(int i = 0; i<16;i++) auxiliar_object->mzptr->matrize[i] = 0.0;
            auxiliar_object->mzptr->matrize[0] = 1.0;
            auxiliar_object->mzptr->matrize[5] = 1.0;
            auxiliar_object->mzptr->matrize[10] = 1.0;
            auxiliar_object->mzptr->matrize[15] = 1.0;
            auxiliar_object->next = _first_object;
            _first_object = auxiliar_object;
            _selected_object = _first_object;
            printf("%s\n",KG_MSSG_FILEREAD);
            break;
        }
        break;

    case 9: /* <TAB> */
        if(_selected_object != 0){
            _selected_object = _selected_object->next;
            /*The selection is circular, thus if we move out of the list we go back to the first element*/
            if (_selected_object == 0) _selected_object = _first_object;
        }else{
            printf("Ez dakagu objekturik, beraz ezin da hurrengo objektua aukeratu\n\n");
        }
        break;

    case 127: /* <SUPR> */
        // TODO if there is not any object in the object list we should not try to free any memory!!!
        if(_selected_object != 0){
            /*Erasing an object depends on whether it is the first one or not*/
             if (_selected_object == _first_object){
                /*To remove the first object we just set the first as the current's next*/
                _first_object = _first_object->next;
                /*Once updated the pointer to the first object it is save to free the memory*/
                // TODO free the auxiliar memory used inside the structure
                free_object(_selected_object);
                free(_selected_object);
                /*Finally, set the selected to the new first one*/
                _selected_object = _first_object;
            } else {
                /*In this case we need to get the previous element to the one we want to erase*/
                auxiliar_object = _first_object;
                while (auxiliar_object->next != _selected_object)
                    auxiliar_object = auxiliar_object->next;
                /*Now we bypass the element to erase*/
                auxiliar_object->next = _selected_object->next;
                /*free the memory*/
                // TODO free the auxiliar memory used inside the structure
                free_object(_selected_object);
                free(_selected_object);
                /*and update the selection. It is the previous to the deleted one. */
                _selected_object = auxiliar_object;
            }
        }else{
            printf("Ezin da objekturik ezabatu ez badago objekturik kargatuta\n\n");
        }
        break;

    case '-':   /* Decrease the projection plane; compute the new dimensions*/
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL){ 
            // with or without pressing CTRL key 
            }
            
            wd=(_ortho_x_max-_ortho_x_min)*KG_STEP_ZOOM;
            he=(_ortho_y_max-_ortho_y_min)*KG_STEP_ZOOM;
            /*In order to avoid moving the center of the plane, we get its coordinates*/
            midx = (_ortho_x_max+_ortho_x_min)/2;
            midy = (_ortho_y_max+_ortho_y_min)/2;
            /*The the new limits are set, keeping the center of the plane*/
            _ortho_x_max = midx + wd/2;
            _ortho_x_min = midx - wd/2;
            _ortho_y_max = midy + he/2;
            _ortho_y_min = midy - he/2;
        
        break;

    case '+':
        //INPLEMENTA EZAZU + FUNTZIOANLITATEA
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL){ 
            // with or without pressing CTRL key 
            }
            
            wd=(_ortho_x_max-_ortho_x_min)/KG_STEP_ZOOM;
            he=(_ortho_y_max-_ortho_y_min)/KG_STEP_ZOOM;
            /*In order to avoid moving the center of the plane, we get its coordinates*/
            midx = (_ortho_x_max+_ortho_x_min)*2;
            midy = (_ortho_y_max+_ortho_y_min)*2;
            /*The the new limits are set, keeping the center of the plane*/
            _ortho_x_max = midx + wd*2;
            _ortho_x_min = midx - wd*2;
            _ortho_y_max = midy + he*2;
            _ortho_y_min = midy - he*2;
        break;

    case '?':
        print_help();
        break;
    case 27: /* <ESC> */
        exit(0);
        break;
    case 'I':
    case 'i':
        if(_selected_object != 0){
            printf("Objektuaren informazioa: \n\n");
            printf("Erpin kopurua:   %d\n", _selected_object->num_vertices);
            printf("Aurpegi kopurua: %d\n", _selected_object->num_faces);
            //printf("Objektuaren izena: %s\n", _selected_object->filename);
        }else{
            printf("Ez dago objekturik kargaturik\n");
        }
        break;
    /*Aldaketa motak*/
    case 'T':
    case 't':
        aldaketa_mota = 't';
        printf("Erreferentzi sistema: %c, Zer aldatu: %c, Aldaketa mota: %c\n", erreferentzi_sistema, zer_aldatu, aldaketa_mota);
        break;
    case 'R':
    case 'r':
        aldaketa_mota = 'r';
        printf("Erreferentzi sistema: %c, Zer aldatu: %c, Aldaketa mota: %c\n", erreferentzi_sistema, zer_aldatu, aldaketa_mota);
        break;
    case 'E':
    case 'e':
        aldaketa_mota = 'e';
        printf("Erreferentzi sistema: %c, Zer aldatu: %c, Aldaketa mota: %c\n", erreferentzi_sistema, zer_aldatu, aldaketa_mota);
        break;
    /*Erreferentzi sistemak*/
    case 'G':
    case 'g':
        erreferentzi_sistema = 'g';
        printf("Erreferentzi sistema: %c, Zer aldatu: %c, Aldaketa mota: %c\n", erreferentzi_sistema, zer_aldatu, aldaketa_mota);
        break;
    case 'L':
    case 'l':
        if(erreferentzi_sistema == 'l'){
            zer_aldatu = 'a';
            printf("Erreferentzi sistema: %c, Zer aldatu: %c, Aldaketa mota: %c\n", erreferentzi_sistema, zer_aldatu, aldaketa_mota);
        }else{
            erreferentzi_sistema = 'l';
            printf("Erreferentzi sistema: %c, Zer aldatu: %c, Aldaketa mota: %c\n", erreferentzi_sistema, zer_aldatu, aldaketa_mota);
        }
        break;

    case 'O':
    case 'o':
        zer_aldatu = 'o';
        printf("Erreferentzi sistema: %c, Zer aldatu: %c, Aldaketa mota: %c\n", erreferentzi_sistema, zer_aldatu, aldaketa_mota);
        break;
    case 'C':
    case 'c':
        zer_aldatu = 'k';
        printf("Erreferentzi sistema: %c, Zer aldatu: %c, Aldaketa mota: %c\n", erreferentzi_sistema, zer_aldatu, aldaketa_mota);
        break;
    case 'Z':
    case 'z':
        if(_selected_object != 0){
            if(_selected_object->mzptr->next != 0){
                mzptr2 = _selected_object->mzptr;
                _selected_object->mzptr = mzptr2->next;
                free(mzptr2);
            }else{
                printf("Ez dago desegiteko aldaketarik\n");
            }
        }else{
            printf("Ez dago objekturik kargatuta\n");
        }
        break;
    default:
        /*In the default case we just print the code of the key. This is usefull to define new cases*/
        printf("%d %c\n", key, key);
    }
    /*In case we have do any modification affecting the displaying of the object, we redraw them*/
    glutPostRedisplay();
}

/**
 * @brief Callback function to control the special keys
 * @param key Key that has been pressed
 * @param x X coordinate of the mouse pointer when the key was pressed
 * @param y Y coordinate of the mouse pointer when the key was pressed
 */
void tekla_berezien_arretarako_funtzioa(int key, int x, int y){
    
    double Mald[16];
    double Mberria[16];
    mz *matberria;
    int i;

    switch (key){
    case GLUT_KEY_UP: /*Gorako gezia*/
        if(_selected_object != 0){
            switch(zer_aldatu){
                case 'o':
                    switch(aldaketa_mota){
                        case 't':
                            lortu_traslazio_matrizea(&(Mald[0]), 0.0, 1.0, 0.0);
                            break;
                        case 'r':
                            lortu_biratu_matrizea(&(Mald[0]), -1.0, 0.0, 0.0);
                            break;
                        case 'e':
                            lortu_eskalatu_matrizea(&(Mald[0]), 1.0, 0.75, 1.0);
                            break;
                        default:
                            break;
                    }
                    break;
                case 'k':

                    break;
                case 'a':

                    break;
                default:
                    break;
            }
        }else{
            printf("Ez dago objekturik zerrendan\n");
        }
        break;
    case GLUT_KEY_DOWN: /*Beherako gezia*/
        if(_selected_object != 0){
            switch(zer_aldatu){
                case 'o':
                    switch(aldaketa_mota){
                        case 't':
                            lortu_traslazio_matrizea(&(Mald[0]), 0.0, -1.0, 0.0);
                            break;
                        case 'r':
                            lortu_biratu_matrizea(&(Mald[0]), 1.0, 0.0, 0.0);
                            break;
                        case 'e':
                            lortu_eskalatu_matrizea(&(Mald[0]), 1.0, 1.333333, 1.0);
                            break;
                        default:
                            break;
                    }
                    break;
                case 'k':

                    break;
                case 'a':

                    break;
                default:
                    break;
            }
        }else{
            printf("Ez dago objekturik zerrendan\n");
        }
        break;
    case GLUT_KEY_RIGHT: /*Eskubirako gezia*/
        if(_selected_object != 0){
            switch(zer_aldatu){
                case 'o':
                    switch(aldaketa_mota){
                        case 't':
                            lortu_traslazio_matrizea(&(Mald[0]), 1.0, 0.0, 0.0);
                            break;
                        case 'r':
                            lortu_biratu_matrizea(&(Mald[0]), 0.0, 1.0, 0.0);
                            break;
                        case 'e':
                            lortu_eskalatu_matrizea(&(Mald[0]), 0.75, 1.0, 1.0);
                            break;
                        default:
                            break;
                    }
                    break;
                case 'k':

                    break;
                case 'a':

                    break;
                default:
                    break;
            }
        }else{
            printf("Ez dago objekturik zerrendan\n");
        }
        break;
    case GLUT_KEY_LEFT: /*Ezkerrerako gezia*/
        if(_selected_object != 0){
            switch(zer_aldatu){
                case 'o':
                    switch(aldaketa_mota){
                        case 't':
                            lortu_traslazio_matrizea(&(Mald[0]), -1.0, 0.0, 0.0);
                            break;
                        case 'r':
                            lortu_biratu_matrizea(&(Mald[0]), 0.0, -1.0, 0.0);
                            break;
                        case 'e':
                            lortu_eskalatu_matrizea(&(Mald[0]), 1.333333, 1.0, 1.0);
                            break;
                        default:
                            break;
                    }
                    break;
                case 'k':

                    break;
                case 'a':

                    break;
                default:
                    break;
            }
        }else{
            printf("Ez dago objekturik zerrendan\n");
        }
        break;
    case GLUT_KEY_PAGE_UP: /*REPAG*/
        if(_selected_object != 0){
            switch(zer_aldatu){
                case 'o':
                    switch(aldaketa_mota){
                        case 't':
                            lortu_traslazio_matrizea(&(Mald[0]), 0.0, 0.0, 1.0);
                            break;
                        case 'r':
                            lortu_biratu_matrizea(&(Mald[0]), 0.0, 0.0, -1.0);
                            break;
                        case 'e':
                            lortu_eskalatu_matrizea(&(Mald[0]), 1.0, 1.0, 1.333333);
                            break;
                        default:
                            break;
                    }
                    break;
                case 'k':

                    break;
                case 'a':

                    break;
                default:
                    break;
            }
        }else{
            printf("Ez dago objekturik zerrendan\n");
        }
        break;
    case GLUT_KEY_PAGE_DOWN: /*AVPAG*/
        if(_selected_object != 0){
            switch(zer_aldatu){
                case 'o':
                    switch(aldaketa_mota){
                        case 't':
                            lortu_traslazio_matrizea(&(Mald[0]), 0.0, 0.0, -1.0);
                            break;
                        case 'r':
                            lortu_biratu_matrizea(&(Mald[0]), 0.0, 0.0, 1.0);
                            break;
                        case 'e':
                            lortu_eskalatu_matrizea(&(Mald[0]), 1.0, 1.0, 0.75);
                            break;
                        default:
                            break;
                    }
                    break;
                case 'k':

                    break;
                case 'a':

                    break;
                default:
                    break;
            }
        }else{
            printf("Ez dago objekturik zerrendan\n");
        }
        break;
    }
    if(_selected_object != 0){
        if(erreferentzi_sistema == 'g') {/*Globala*/
            ezkerretik_biderkatu(&(Mberria[0]),&(Mald[0]),_selected_object->mzptr->matrize);
        }else{/*Lokala*/
            eskuinetik_biderkatu(&(Mberria[0]),&(Mald[0]),_selected_object->mzptr->matrize);
        }
        matberria = (mz *)malloc(sizeof(mz));
        for(i = 0; i<16; i++) matberria->matrize[i] = Mberria[i];
        matberria->next = _selected_object->mzptr;
        _selected_object->mzptr = matberria;
        printf("%f, %f, %f, %f\n", matberria->matrize[0], matberria->matrize[4], matberria->matrize[8], matberria->matrize[12]);
        printf("%f, %f, %f, %f\n", matberria->matrize[1], matberria->matrize[5], matberria->matrize[9], matberria->matrize[13]);
        printf("%f, %f, %f, %f\n", matberria->matrize[2], matberria->matrize[6], matberria->matrize[10], matberria->matrize[14]);
        printf("%f, %f, %f, %f\n\n\n", matberria->matrize[3], matberria->matrize[7], matberria->matrize[11], matberria->matrize[15]);
        glutPostRedisplay();
    }

}

