#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include<iostream>
#include<stdlib.h>

using namespace std;
using namespace sf;




short alternancia=1,cambio_alternancia=0;

// Estructura de Caballos con el Sprite correspondiente, su textura y su estatus

struct Caballos{
    Sprite imagen;
    Texture textura;
    int vivo=0;
};

// Instanciación de la estructura Caballos

struct Caballos *caba = new struct Caballos[6];
struct Caballos *pcaba=caba;

// Estructura de Alfiles con el Sprite correspondiente, su textura y su estatus

struct Alfiles{
    Sprite imagen;
    Texture textura;
    int vivo=0;
};

// Instanciacion de la estructura Alfiles

struct Alfiles *alfi = new struct Alfiles[4];
struct Alfiles *palfi=alfi;

// Estructura de la Cara de los Dados con el sprite correspondiente y su textura

struct Car_dados{
    Sprite imagen;
    Texture textura;
};

// Estructura para los dos Dados que se muestran en pantalla

struct Dado{
    struct Car_dados *carac = new struct Car_dados[10];
    struct Car_dados *pcarac = carac;
};

// Estructura del Tablero con el numero, el contenido y el apuntador a la siguiente casilla

struct NODO{
    char casilla;
    int num;
    NODO *sig;
};

class tablero{
    NODO *cabeza;
public:
    tablero(){cabeza = NULL;}
    
    void crear_tablero();
    
    struct NODO* obt_pos(int n);
    
    char getcasilla(int n);
    void setcasilla(char cas,int n){(obt_pos(n))->casilla = cas;}
    
    int getnum(int n){return (obt_pos(n))->num;}
    void setnum(int n){(obt_pos(n))->num = n;}
    
    struct NODO *getsig(int n){return (obt_pos(n))->sig;}
    void setsig(struct NODO *s,int n){(obt_pos(n))->sig = s;}
    
    ~tablero();
};


// Instanciacion de la estructura Dados

struct Dado *dados = new struct Dado[2];
struct Dado *pdados= dados;

// Definicion de las funciones utilizadas a lo largo del programa

int colocar_fichas(int fichas_puestas, tablero *ptab);
int mov_pos_cab(int pos, int rand, int ficha,tablero *ptab);
void mov_cab(tablero *ptab);
int mov_pos_alf(int pos, int rand, int ficha,tablero *ptab);
void mov_alf(tablero *ptab);
int leer_vector(int cont_fichas,tablero *ptab);
void Actualizar_dados(int fase);

// Variables globales utilizadas a lo largo del programa

int ficha_real_elegida, movimiento_real_elegido,ficha_opc_elegida, movimiento_opc_elegido;
int primer_turno=0,cont_com = 0, ganador=0,razon_victoria=0;
Sprite ultimox,ultimoy;
char cab = 49, alf = 65;

int main(int, char const**)
{
    tablero tab;
    tab.crear_tablero();
    //Verifica que se halla reservado espacio de memoria con exito
    if(alfi == NULL || caba == NULL)
        exit(1);
    
    //Inicializamos en ceros el miembro vivo de las estructuras del alfil y el caballo
    for(int x = 0; x < 6; x++){
        
        if(x < 4)
            (palfi + x)->vivo= 0;
        (pcaba + x) -> vivo = 0;
    }
    
    // Se crean variables utilizadas posteriormente en el codigo
    int cont_fichas=0, cont_turnos=0, cont_alfiles_vivos=0, cont_caballos_vivos=0;
    float pos_x, pos_y;
    int pos_inicial;
    srand(time(NULL));
    
    // Creamos la ventana principal
    RenderWindow window(VideoMode(1250, 850), "Decentes contra la corrupcion");
    
    
    // Carga la textura del pergamino
    Texture textura_pergamino;
    if (!textura_pergamino.loadFromFile( "imag/p1.png")) {
        return EXIT_FAILURE;
    }
    
    Sprite pergamino(textura_pergamino);
    
    //Cargamos la textura del tablero
    Texture textura_tablero;
    if (!textura_tablero.loadFromFile( "imag/chessboard.jpg")) {
        return EXIT_FAILURE;
    }
    //asignamos la textura del tablero a un "sprite"
    Sprite tablero(textura_tablero);
    
    //Cargamos la texturas de los alfiles
    char *texto_alf = new char[18];
    
    //Verifica que se halla reservado espacio de memoria con exito
    if(texto_alf == NULL)
        exit(1);
    
    // Declara posicion a posicion los caracteres de vector texto_alf
    (*(texto_alf + 0)) = 'I';
    (*(texto_alf + 1)) = 'm';
    (*(texto_alf + 2)) = 'a';
    (*(texto_alf + 3)) = 'g';
    (*(texto_alf + 4)) = '/';
    (*(texto_alf + 5)) = 'a';
    (*(texto_alf + 6)) = 'l';
    (*(texto_alf + 7)) = 'f';
    (*(texto_alf + 8)) = 'I';
    (*(texto_alf + 9)) = 'l';
    (*(texto_alf + 10)) = '(';
    (*(texto_alf + 11)) = '1';
    (*(texto_alf + 12)) = ')';
    (*(texto_alf + 13)) = '.';
    (*(texto_alf + 14)) = 'p';
    (*(texto_alf + 15)) = 'n';
    (*(texto_alf + 16)) = 'g';
    (*(texto_alf + 17)) = '\0';
    
    //cargamos la textura de cada alfil
    for(int x = 0; x < 4; x++){
        if (!((palfi+ x)->textura).loadFromFile( texto_alf ))
            return EXIT_FAILURE;
        (*(texto_alf + 11))++;
    }
    
    //Cargamos la texturas de los caballos
    char *texto_cab = new char[14];
    
    //Verifica que se halla reservado espacio de memoria con exito
    if(texto_cab == NULL)
        exit(1);
    
    // Declara posicion a posicion los caracteres de vector texto_cab
    (*(texto_cab + 0))= 'i';
    (*(texto_cab + 1)) = 'm';
    (*(texto_cab + 2)) = 'a';
    (*(texto_cab + 3)) = 'g';
    (*(texto_cab + 4)) = '/';
    (*(texto_cab + 5)) = 'c';
    (*(texto_cab + 6)) = 'a';
    (*(texto_cab + 7)) = 'b';
    (*(texto_cab + 8)) = '1';
    (*(texto_cab + 9)) = '.';
    (*(texto_cab + 10)) = 'p';
    (*(texto_cab + 11)) = 'n';
    (*(texto_cab + 12)) = 'g';
    (*(texto_cab + 13)) = '\0';
    
    //Cargamos la textura de cada caballo
    for(int x = 0; x < 6; x++){
        if(!((pcaba+x)->textura).loadFromFile(texto_cab))
            return EXIT_FAILURE;
        (*(texto_cab + 8))++;
    }
    
    //Cargamos las texturas de los dados
    char *texto_dado = new char[15];
    
    //Verifica que se halla reservado espacio de memoria con exito
    if(texto_dado == NULL)
        exit(1);
    
    // Declara posicion a posicion los caracteres de vector texto_dado
    (*(texto_dado + 0)) = 'i';
    (*(texto_dado + 1)) = 'm';
    (*(texto_dado + 2)) = 'a';
    (*(texto_dado + 3)) = 'g';
    (*(texto_dado + 4)) = '/';
    (*(texto_dado + 5)) = 'd';
    (*(texto_dado + 6)) = 'a';
    (*(texto_dado + 7)) = 'd';
    (*(texto_dado + 8)) = 'o';
    (*(texto_dado + 9)) = '0';
    (*(texto_dado + 10)) = '.';
    (*(texto_dado + 11)) = 'p';
    (*(texto_dado + 12)) = 'n';
    (*(texto_dado + 13)) = 'g';
    (*(texto_dado + 14)) = '\0';
    
    //Cargamos las texturas de cada dado
    for(int x = 0; x < 10; x++){
        if (!(((dados) -> pcarac + x) -> textura).loadFromFile( texto_dado ))
            return EXIT_FAILURE;
        if (!(((dados + 1) -> pcarac + x) -> textura).loadFromFile( texto_dado ))
            return EXIT_FAILURE;
        
        (*(texto_dado + 9))++;
    }
    
    //asignamos la textura de los alfiles a los "sprite"
    
    for(int x = 0; x < 4 ;x++){
        ((palfi + x)->imagen).setTexture((palfi + x)->textura);
    }
    
    //asignamos la textura de los caballos a los "sprite"
    
    for(int x = 0; x < 6 ;x++){
        ((pcaba+x)->imagen).setTexture((pcaba+x)->textura);
    }
    
    //asignamos la textura de los dados a los "sprite"
    for(int x = 0; x < 10 ;x++){
        (((dados) -> pcarac + x) -> imagen).setTexture(((dados) -> pcarac + x) -> textura);
        (((dados) -> pcarac + x) -> imagen).setPosition(900,75);
        
        (((dados + 1) -> pcarac + x) -> imagen).setTexture((((dados + 1) -> pcarac + x) -> textura));
        (((dados + 1) -> pcarac + x) -> imagen).setPosition(1100,75);
    }
    
    //Asignamos una posicion inicial para cada uno de los dados y les damos una textura inicial
    ultimox.setTexture((((dados) -> pcarac) -> textura));
    ultimox.setPosition(900,75);
    
    ultimoy.setTexture((((dados) -> pcarac + 1) -> textura));
    ultimoy.setPosition(1100,75);
    
    // cargamos un tipo particular de letra
    Font font_inicial;
    if (!font_inicial.loadFromFile( "Fuentes/sansation.ttf")) {
        return EXIT_FAILURE;
    }
    
    Font font_victoria;
    if (!font_victoria.loadFromFile( "Fuentes/Moon.ttf")) {
        return EXIT_FAILURE;
    }
    
    //escribimos coordenadas guia y asignamos el tipo de letra font_inicial
    
    Text *coord = new Text[9],*coord2 = new Text[9];
    
    //Verifica que se halla reservado espacio de memoria con exito
    if(coord == NULL || coord2 == NULL )
        exit(1);
    
    //Posicionamos las coordenadas del tablero
    char ref_vertical = 56, ref_horizontal = 97;
    for(int x = 1; x < 9 ; x++){
        (*(coord + x)).setString(ref_vertical);
        (*(coord + x)).setFont(font_inicial);
        (*(coord + x)).setCharacterSize(35);
        ref_vertical--;
        
        (*(coord2 + x)).setString(ref_horizontal);
        (*(coord2 + x)).setFont(font_inicial);
        (*(coord2 + x)).setCharacterSize(35);
        ref_horizontal++;
        
        (*(coord + x)).setPosition(810, 30 + 100*(x-1));
        (*(coord2 + x)).setPosition(40 + 100*(x - 1),800);
    }
    
    //escribimos los textos que necesitaremos en ciertos puntos del juego
    Text texto_inicio("Turnemos un decente y un corrupto\nen la poscicion:", font_inicial, 20);
    Text juegan_negras("turno corruptos",font_inicial, 20);
    Text juegan_blancas("turno decentes",font_inicial,20);
    
    
    //textos movimiento imposible caballo
    char num_cab[47] = {'E','l',' ','c','a','b','a','l','l','o',' ','1',' ','n','o',' ','p','u','e','d','e',' ','r','e','a','l','i','z','a','r',' ','e','l',' ','m','o','v','i','m','i','e','n','t','o',' ','1','\0'};
    Text imposible_cab(num_cab, font_inicial, 20);
    
    //texto movimientos imposible alfil
    char num_alf[45] = {'E','l',' ','a','l','f','i','l',' ','1',' ','n','o',' ','p','u','e','d','e',' ','r','e','a','l','i','z','a','r',' ','e','l',' ','m','o','v','i','m','i','e','n','t','o',' ','1','\0'};
    Text imposible_a1_m1(num_alf, font_inicial, 20);
    
    //ponemos color al texto y posicionamos el texto donde queremos
    
    texto_inicio.setPosition(900,25);
    juegan_negras.setPosition(980,820);
    juegan_blancas.setPosition(980,820);
    
    // Cargamos la musica del juego
    Music music;
    if (!music.openFromFile( "Sonido/Dark_Souls_III.ogg")) {}
    
    // Iniciamos la musica
    music.play();
    
    // Iniciamos el ciclo del juego
    while (window.isOpen()){
        // Procesa eventos
        Event event;
        while (window.pollEvent(event)){
            switch(event.type){
                    //si se cierra la ventana manualmente
                case (Event::Closed):
                    window.close();
                    //procesa el evento de una tecla opresionada
                case (Event::KeyReleased):
                    // Si se presiona escape, se cierra
                    if(event.key.code == Keyboard::Escape)
                        window.close();
                    
                    if (event.key.code == Keyboard::Up){
                        
                        if(cambio_alternancia % 2 ==0 )alternancia=1;
                        else alternancia=0;
                        
                        cambio_alternancia++;
                        
                        if (alternancia!=1 || primer_turno == 0){
                        
                        //Si ganador = 1 se cierra la ventana
                        if(ganador == 1)
                            window.close();
                        
                        if(cont_fichas<10){
                            
                            // Ubica las fichas en base a cuantas ya han sido colocadas
                            //esta funcion retorna la posicion en el vector de la siguiente ficha
                            cout<<"habran menos de 10 fichas?"<<endl;
                            pos_inicial=colocar_fichas(cont_fichas,&tab);
                            //formula para transformar posicion del vector a pixeles en la ventanaen sentido x, y
                            pos_x=(int(pos_inicial-int(pos_inicial/8)*8)*100);
                            pos_y=(int((pos_inicial/8)*100));
                            //posiciona la ficha correspondiente a la iteracion en las coordenadas previamente calculadas
                            if(cont_fichas==0)
                                ((pcaba)->imagen).setPosition(pos_x,pos_y);
                            if(cont_fichas==1)
                                ((palfi)->imagen).setPosition(pos_x,pos_y);
                            if(cont_fichas==2)
                                ((pcaba+1)->imagen).setPosition(pos_x,pos_y);
                            if(cont_fichas==3)
                                ((palfi+1)->imagen).setPosition(pos_x,pos_y);
                            if(cont_fichas==4)
                                ((pcaba+2)->imagen).setPosition(pos_x,pos_y);
                            if(cont_fichas==5)
                                ((palfi+2)->imagen).setPosition(pos_x,pos_y);
                            if(cont_fichas==6)
                                ((pcaba+3)->imagen).setPosition(pos_x,pos_y);
                            if(cont_fichas==7)
                                ((palfi+3)->imagen).setPosition(pos_x,pos_y);
                            if(cont_fichas==8)
                                ((pcaba+4)->imagen).setPosition(pos_x,pos_y);
                            if(cont_fichas==9)
                                ((pcaba+5)->imagen).setPosition(pos_x,pos_y);
                            //aumentamos el contador de fichas ya puestas
                            ++cont_fichas;
                            //animacion dados
                            for(int repeticiones=0;repeticiones<=20;repeticiones++){
                                for(int x = 0; x < 9; x++){
                                    window.draw((((dados) -> pcarac + x) -> imagen));
                                    if(x > 5)
                                        window.draw((((dados + 1 ) -> pcarac + x - 5) -> imagen));
                                    window.display();
                                }
                                
                            }
                            /*
                             sacamos el primer digito del numero al azar entre 1 y 64, lo asignamos
                             al sprite "ultimoy" que es el segundo dado de izquierda a derecha que
                             se va a quedar quieto hasta que la tecla arriba vuelva a ser presionada
                             */
                            /*
                             sacamos el segundo digito del numero al azar entre 1 y 64, lo asignamos
                             al sprite "ultimox" que es el primer dado de izquierda a derecha que
                             se va a quedar quieto hasta que la tecla arriba vuelva a ser presionada
                             */
                            
                            if(pos_inicial%10 == 9){
                                ultimoy.setTexture((((dados) -> pcarac) -> textura));
                                ultimox.setTexture((((dados) -> pcarac + (pos_inicial/10) + 1) -> textura));
                            }
                            else {
                                ultimoy.setTexture((((dados) -> pcarac + (pos_inicial%10) + 1) -> textura));
                                ultimox.setTexture((((dados) -> pcarac + (pos_inicial/10)) -> textura));
                            }
                            
                            // si es la ultima iteracion poniendo fichas, empezamos contador de las que estan vivas
                            if (cont_fichas==10){
                                cont_alfiles_vivos=4;
                                cont_caballos_vivos=6;
                                
                            }
                        }
                        
                        else{
                            //si se va a decidir el primer turno, entra aqui
                            if(cont_turnos==0){
                                
                                primer_turno = rand() % 2 + 1; //al azar, turno 1 o 2 (decentes o corruptos)
                                //dependinedo del numero ponemos el dado que va a quedarse quieto
                                if(primer_turno==1){
                                    ultimox.setTexture((((dados) -> pcarac) -> textura));
                                    ultimoy.setTexture((((dados) -> pcarac + 1) -> textura));
                                }
                                else if(primer_turno==2){
                                    ultimox.setTexture((((dados) -> pcarac) -> textura));
                                    ultimoy.setTexture((((dados) -> pcarac + 2) -> textura));
                                }
                                cont_turnos=primer_turno; //nuestro contador de turno va ser igual al primer turno (1 o 2)
                                
                                //animacion dados
                                for(int repeticiones=0;repeticiones<=20;repeticiones++){
                                    for(int x = 0; x < 9; x++){
                                        window.draw((((dados + 1) -> pcarac + x) -> imagen));
                                        if(x > 5)
                                            window.draw((((dados + 1) -> pcarac + x - 5) -> imagen));
                                        window.display();
                                    }
                                }
                            }
                            
                            //si el turno es par y estamos en la fase buscar ficha del turno corruptos(alfiles) entra aqui
                            else if(cont_turnos%2==0){
                                mov_alf(&tab);
                                
                                //animacion dados
                                for(int repeticiones=0;repeticiones<=20;repeticiones++){
                                    for(int x = 0; x < 9; x++){
                                        (((dados) -> pcarac + x) -> imagen).setPosition(900,75);
                                        window.draw((((dados) -> pcarac + x) -> imagen));
                                        (((dados) -> pcarac + x) -> imagen).setPosition(900,220);
                                        window.draw((((dados) -> pcarac + x) -> imagen));
                                        
                                        if(x > 5){
                                            (((dados+ 1) -> pcarac + x) -> imagen).setPosition(1100,75);
                                            window.draw((((dados + 1) -> pcarac + x - 5) -> imagen));
                                            (((dados + 1) -> pcarac + x) -> imagen).setPosition(1100,220);
                                            window.draw((((dados + 1) -> pcarac + x - 5) -> imagen));
                                        }
                                        
                                        else{
                                            (((dados + 1) -> pcarac + x) -> imagen).setPosition(1100,75);
                                            window.draw((((dados + 1) -> pcarac + x) -> imagen));
                                            (((dados + 1) -> pcarac + x) -> imagen).setPosition(1100,220);
                                            window.draw((((dados + 1) -> pcarac + x) -> imagen));
                                        }
                                        window.display();
                                    }
                                }
                                
                                //asignamos los dados que se muestran dependiendo del numero al azar
                                cont_turnos++;
                            }
                            
                            
                            else if(cont_turnos%2==1 ){
                                mov_cab(&tab);
                                
                                //animacion dados
                                for(int repeticiones=0;repeticiones<=20;repeticiones++){
                                    for(int x = 0; x < 9; x++){
                                        (((dados) -> pcarac + x) -> imagen).setPosition(900,75);
                                        window.draw((((dados) -> pcarac + x) -> imagen));
                                        (((dados) -> pcarac + x) -> imagen).setPosition(900,220);
                                        window.draw((((dados) -> pcarac + x) -> imagen));
                                        
                                        if(x > 5){
                                            (((dados + 1) -> pcarac + x) -> imagen).setPosition(1100,75);
                                            window.draw((((dados + 1) -> pcarac + x - 5) -> imagen));
                                            (((dados + 1) -> pcarac + x) -> imagen).setPosition(1100,220);
                                            window.draw((((dados + 1) -> pcarac + x - 5) -> imagen));
                                        }
                                        
                                        else{
                                            (((dados + 1) -> pcarac + x) -> imagen).setPosition(1100,75);
                                            window.draw((((dados + 1) -> pcarac + x) -> imagen));
                                            (((dados + 1) -> pcarac + x) -> imagen).setPosition(1100,220);
                                            window.draw((((dados + 1) -> pcarac + x) -> imagen));
                                        }
                                        window.display();
                                    }
                                }
                                //asignamos los dados que se muestran dependiendo del numero al azar
                                cont_turnos++;
                            }
                        }
                            
                            
                        }
                        else{
                            
                            
                            
                            if(cont_turnos > primer_turno){
                                
                                if(cont_turnos>0 && cont_turnos%2==0){
                                    
                                    window.draw(juegan_blancas);
                                    
                                    texto_inicio.setPosition(900,40);
                                    window.draw(texto_inicio);
                                    Actualizar_dados(1);
                                    window.draw(ultimox);
                                    window.draw(ultimoy);
                                    
                                    texto_inicio.setPosition(900,180);
                                    window.draw(texto_inicio);
                                    Actualizar_dados(2);
                                    window.draw(ultimox);
                                    window.draw(ultimoy);
                                    
                                    texto_inicio.setPosition(900,460);
                                    window.draw(texto_inicio);
                                    Actualizar_dados(3);
                                    window.draw(ultimox);
                                    window.draw(ultimoy);
                                    
                                    texto_inicio.setPosition(900,620);
                                    window.draw(texto_inicio);
                                    Actualizar_dados(4);
                                    window.draw(ultimox);
                                    window.draw(ultimoy);
                                }
                                
                                
                                if(cont_turnos>0 && cont_turnos%2==1){
                                    
                                    window.draw(juegan_negras);
                                    texto_inicio.setPosition(900,40);
                                    window.draw(texto_inicio);
                                    Actualizar_dados(1);
                                    window.draw(ultimox);
                                    window.draw(ultimoy);
                                    
                                    texto_inicio.setPosition(900,180);
                                    window.draw(texto_inicio);
                                    Actualizar_dados(2);
                                    window.draw(ultimox);
                                    window.draw(ultimoy);
                                    
                                    texto_inicio.setPosition(900,460);
                                    window.draw(texto_inicio);
                                    Actualizar_dados(3);
                                    window.draw(ultimox);
                                    window.draw(ultimoy);
                                    
                                    texto_inicio.setPosition(900,620);
                                    window.draw(texto_inicio);
                                    Actualizar_dados(4);
                                    window.draw(ultimox);
                                    window.draw(ultimoy);
                                }
                            }
                            
                        }
                        //////
                    }
            }
        }
        // Borramos pantalla
        window.clear();
        window.draw(tablero);
        razon_victoria=leer_vector(cont_fichas,&tab);
        
        //Dibujamos los textos y dados correspondientes junto con el tablero
        window.draw(texto_inicio);
        window.draw((((dados) -> pcarac) -> imagen));
        window.draw((((dados + 1) -> pcarac) -> imagen));
      
    
        
        // escribe números guia del tablero a la izquierda
        for(int x = 1; x < 9; x++){
            window.draw((*(coord + x)));
            window.draw((*(coord2 + x)));
        }
        
        //dibuja los alfiles
        for(int x = 0; x < 4; x++){
            if(((palfi+x)->vivo) == 1)
                window.draw((palfi + x)->imagen);
        }
        
        //dibuja los caballos
        for(int x = 0; x < 6; x++){
            if(((pcaba+x)->vivo) == 1)
                window.draw((pcaba+x)->imagen);
        }
    
        //Dibuja los textos, dados e instrucciones para saber quien inicia el juego
        if(cont_fichas>0 && cont_fichas<10)
            window.draw(texto_inicio);
        if(cont_fichas>=10 && cont_turnos==primer_turno){
            texto_inicio.setString("lancemos los dados y veamos \nquien empieza:");
            texto_inicio.setPosition(900, 25);
            window.draw(texto_inicio);
            texto_inicio.setPosition(900, 180);
            texto_inicio.setString("1: Decentes    2: Corruptos");
            window.draw(texto_inicio);
        }
         
        
        //Dibuja los textos, dados e instrucciones correspondientes a cada turno
        if(cont_turnos > primer_turno){
            
            if(cont_turnos>0 && cont_turnos%2==0){
                
                window.draw(juegan_blancas);
                texto_inicio.setPosition(900,40);
                texto_inicio.setString("El Decente elegido al azar es:");
                window.draw(texto_inicio);
                Actualizar_dados(1);
                window.draw(ultimox);
                window.draw(ultimoy);
                
                texto_inicio.setPosition(900,180);
                texto_inicio.setString("El movimento elegido al azar es:");
                window.draw(texto_inicio);
                Actualizar_dados(2);
                window.draw(ultimox);
                window.draw(ultimoy);
                
                texto_inicio.setPosition(900,460);
                texto_inicio.setString("El Decente que si ha podido \nrealizar el movimiento es:");
                window.draw(texto_inicio);
                Actualizar_dados(3);
                window.draw(ultimox);
                window.draw(ultimoy);
                
                texto_inicio.setPosition(900,620);
                texto_inicio.setString("El movimiento que si se ha podido\nrealizar es:");
                window.draw(texto_inicio);
                Actualizar_dados(4);
                window.draw(ultimox);
                window.draw(ultimoy);
            }
         
    
            if(cont_turnos>0 && cont_turnos%2==1){
                
                window.draw(juegan_negras);
                texto_inicio.setPosition(900,40);
                texto_inicio.setString("El Corrupto elegido al azar es:");
                window.draw(texto_inicio);
                Actualizar_dados(1);
                window.draw(ultimox);
                window.draw(ultimoy);
                
                texto_inicio.setPosition(900,180);
                texto_inicio.setString("El movimento elegido al azar es:");
                window.draw(texto_inicio);
                Actualizar_dados(2);
                window.draw(ultimox);
                window.draw(ultimoy);
                
                texto_inicio.setPosition(900,460);
                texto_inicio.setString("El Corrupto que si ha podido \nrealizar el movimiento es:");
                window.draw(texto_inicio);
                Actualizar_dados(3);
                window.draw(ultimox);
                window.draw(ultimoy);
                
                texto_inicio.setPosition(900,620);
                texto_inicio.setString("El movimiento que si se ha podido\nrealizar es:");
                window.draw(texto_inicio);
                Actualizar_dados(4);
                window.draw(ultimox);
                window.draw(ultimoy);
            }
        }
        
        //Limita que los dados solo aparezcan una vez
        if(cont_fichas>0)
            window.draw(ultimox);
        
        if(cont_fichas>0)
            window.draw(ultimoy);
        
        //Se asigna el string correspondiente para el texto de victoria
        if (ganador==1){
            switch(razon_victoria){
                    
                case 1:
                    texto_inicio.setString("    GANARON LOS DECENTES PORQUE \n\t\t   YA NO HAY CORRUPTOS");
                    break;
                    
                case 2:
                    texto_inicio.setString("    GANARON LOS CORRUPTOS PORQUE \n\t\t        HAY 3 DECENTES" );
                    break;
                    
                case 3:
                    texto_inicio.setString("    GANARON LOS CORRUPTOS PORQUE \nSUCEDIERON 10 JUGADAS SIN CAPTURAS" );
                    break;
                default:
                    break;
            }
            
            texto_inicio.setFont(font_victoria);
            texto_inicio.setCharacterSize(80);
            texto_inicio.setPosition(250,320);
            pergamino.setOrigin(313.0,246.0);
            pergamino.setPosition(625,425);
            pergamino.setScale(1.7,0.6);
            texto_inicio.setFillColor(Color::Black);
            window.draw(pergamino);
            window.draw(texto_inicio);
            
        }
        // Actualiza la ventana
        window.display();
    }
    return EXIT_SUCCESS;
}


int colocar_fichas(int fichas_puestas, tablero *ptab){
    cout<<"hay menos de 10 fichas"<<endl;
    
    int random64;
    
    //Dependiendo de la cantidad de fichas puestas coloca la siguiente
    if(fichas_puestas>9)
        return fichas_puestas;
    else if (fichas_puestas==9)
        ((pcaba+5)->vivo)=1;
    else if (fichas_puestas==8)
        ((pcaba+4)->vivo)=1;
    else if (fichas_puestas==7)
        ((palfi + 3)->vivo)=1;
    else if (fichas_puestas==6)
        ((pcaba+3)->vivo)=1;
    else if (fichas_puestas==5)
        ((palfi + 2)->vivo)=1;
    else if (fichas_puestas==4)
        ((pcaba+2)->vivo)=1;
    else if (fichas_puestas==3)
        ((palfi + 1)->vivo)=1;
    else if (fichas_puestas==2)
        ((pcaba+1)->vivo)=1;
    else if (fichas_puestas==1)
        ((palfi + 0)->vivo)=1;
    else if (fichas_puestas==0)
        ((pcaba+0)->vivo)=1;
    
    //Se genera los numero al azar donde se posicionaran las fichas
    do{
        random64 = rand() % 64;
    }while(ptab->getcasilla(random64) != 95);
    
    //Verifica si esta colocando un caballo o un alfil
    if(fichas_puestas % 2 == 0){
        ptab->setcasilla(cab,random64);
        cab++;
    }
    
    else{
        if(fichas_puestas > 7 && fichas_puestas < 10){
            ptab->setcasilla(cab,random64);
            cab++;
        }
        else{
            ptab->setcasilla(alf, random64);
            alf++;
        }
    }
    
    int a = 0;
    
    // El ciclo va a recorrer los 64 espacios del vector, en donde cada 8 espacios se realice un salto de linea.
    while(a < 64){
        if(a % 8 == 0)
            cout << endl;
        cout << ptab->getcasilla(a) << " ";
        a++;
    }
    cout << endl;
    return random64; //Retorna la posicion aleatoria
}

//////////////////////////////////////////////////////////////////////
// Funci�n que crea un vector de 64 espacios, representa el tablero //
//////////////////////////////////////////////////////////////////////

void tablero::crear_tablero(){
    struct NODO *aux1 = new struct NODO,*aux2;
    int a=0;
    cabeza = aux1;
    aux1 -> num = a;
    aux1 -> casilla= 95;
    aux1 -> sig = NULL;
    while(a < 63){
        a++;
        aux2 = new struct NODO;
        aux2 -> num = a;
        aux2 -> casilla= 95;
        aux2 -> sig = NULL;
        aux1 -> sig = aux2 ;
        aux1 = aux2;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Funci�n para verificar si el movimiento seleccionado del cabllo es posible de realizar //
/////////////////////////////////////////////////////////////////////////////////////////////

int mov_pos_cab(int pos, int rand, int ficha, tablero *ptab){
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    // Se traen la posici�n del caballo en t�rminos del vector (pos), un n�mero aleatorio que //
    // representa un movimiento (rand) y el entero (ficha)                                   //
    ////////////////////////////////////////////////////////////////////////////////////////////
    
    int posh, col1, fil1, colh, filh, movyes = 0;
    fil1 = pos / 8; // Se calcula la fila de la ficha antes de realizar cualquier movimiento
    col1 = pos - (8*fil1); // Se calcula la columna de la ficha antes de realizar cualquier movimiento
    
    ///////////////////////////////////////////////////////////////////////////////////////
    // El switch empieza a evaluar si el movimiento seleccionado es posible de realizar. //
    // Si este no es posible, recorre los siguientes movimientos hasta que alguno de     //
    // estos sea posible.                                                                //
    ///////////////////////////////////////////////////////////////////////////////////////
    
    switch(rand){
        case 1: // Movimiento 1 del caballo
            posh = pos - 15; // Se calcula la posic�n hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            filh = posh / 8; // Se calcula la fila hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            colh = posh - (8*filh); // Se calcula la columna hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento no es posible de realizar:                               //
            // 1. Si la diferencia entre la fila en la que est� la ficha y la fila hipot�tica es mayor a 2.       //
            // 2. Si la diferencia entre la columna en la que est� la ficha y la columna hipot�tica es mayor a 2. //
            // 3. Si la posici�n hipot�tica es menor a 0 (esta no existe en el vector).                           //
            // 4. Si la posici�n hipot�tica es mayor a 63 (esta no existe en el vector).                          //
            // 5. Si en la posici�n hipot�tica se encuentra otro caballo.                                         //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            if(abs(fil1 - filh) > 2 || abs(col1 - colh) > 2 || posh < 0 || posh > 63 || (ptab->getcasilla(posh) > 48 && ptab->getcasilla(posh) < 55))
                cout << endl <<"el movimiento 1 no es posible "<< endl ;
            
            ///////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento es posible de realizar: //
            // 1. En la posici�n hipot�tica se encuentra un alfil.               //
            // 2. En la posici�n hipot�tica no hay otra ficha.                   //
            ///////////////////////////////////////////////////////////////////////
            
            else if((ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69) || ptab->getcasilla(posh) == 95){
                
                // Verifica si en la jugada se realiz� una captura
                if(ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69){
                    // Si se realiza una captura, se reinicia la variable "cont_com"
                    cont_com = 0;
                    (palfi + ptab->getcasilla(posh) - 65)->vivo = 0;
                }
                else
                    // Si no se realiza una captura, se suma +1 a la variable "cont_com"
                    cont_com++;
                
                /////////////////////////////////////////////////////////////////////////
                // Se realiza la jugada, asignando la ficha a la posici�n hipot�tica y //
                // borrando la ficha de su posici�n anterior.                          //
                // Al ser el movimiento posible, se cierra el switch.                  //
                /////////////////////////////////////////////////////////////////////////
                
                ptab->setcasilla(ficha, posh);
                ptab->setcasilla(95, pos);
                movyes = 1;
                movimiento_real_elegido = 1;
                cout << endl << "El movimiento 1 es posible " << endl;
                break;
            }
            
        case 2: // Movimiento 2 del
            posh = pos - 6; // Se calcula la posic�n hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            filh = posh / 8; // Se calcula la fila hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            colh = posh - (8*filh); // Se calcula la columna hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento no es posible de realizar:                               //
            // 1. Si la diferencia entre la fila en la que est� la ficha y la fila hipot�tica es mayor a 2.       //
            // 2. Si la diferencia entre la columna en la que est� la ficha y la columna hipot�tica es mayor a 2. //
            // 3. Si la posici�n hipot�tica es menor a 0 (esta no existe en el vector).                           //
            // 4. Si la posici�n hipot�tica es mayor a 63 (esta no existe en el vector).                          //
            // 5. Si en la posici�n hipot�tica se encuentra otro caballo.                                         //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            if(abs(fil1 - filh) > 2 || abs(col1 - colh) > 2 || posh < 0 || posh > 63 || (ptab->getcasilla(posh) > 48 && ptab->getcasilla(posh) < 55))
                cout << "el movimiento 2 no es posible ";
            
            ///////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento es posible de realizar: //
            // 1. En la posici�n hipot�tica se encuentra un alfil.               //
            // 2. En la posici�n hipot�tica no hay otra ficha.                   //
            ///////////////////////////////////////////////////////////////////////
            
            else if((ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69) || ptab->getcasilla(posh) == 95){
                
                // Verifica si en la jugada se realiz� una captura
                if(ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69){
                    // Si se realiza una captura, se reinicia la variable "cont_com"
                    cont_com = 0;
                    (palfi+ ptab->getcasilla(posh) - 65)->vivo = 0;
                }
                else
                    // Si no se realiza una captura, se suma +1 a la variable "cont_com"
                    cont_com++;
                
                /////////////////////////////////////////////////////////////////////////
                // Se realiza la jugada, asignando la ficha a la posici�n hipot�tica y //
                // borrando la ficha de su posici�n anterior.                          //
                // Al ser el movimiento posible, se cierra el switch.                  //
                /////////////////////////////////////////////////////////////////////////
                
                ptab->setcasilla(ficha, posh);
                ptab->setcasilla(95, pos);
                movyes = 1;
                movimiento_real_elegido = 2;
                cout << endl << "el movimiento 2 es posible " << endl;
                break;
            }
            
        case 3: // Movimiento 3 del
            posh = pos + 10; // Se calcula la posic�n hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            filh = posh / 8; // Se calcula la fila hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            colh = posh - (8*filh); // Se calcula la columna hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento no es posible de realizar:                               //
            // 1. Si la diferencia entre la fila en la que est� la ficha y la fila hipot�tica es mayor a 2.       //
            // 2. Si la diferencia entre la columna en la que est� la ficha y la columna hipot�tica es mayor a 2. //
            // 3. Si la posici�n hipot�tica es menor a 0 (esta no existe en el vector).                           //
            // 4. Si la posici�n hipot�tica es mayor a 63 (esta no existe en el vector).                          //
            // 5. Si en la posici�n hipot�tica se encuentra otro caballo.                                         //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            if(abs(fil1 - filh) > 2 || abs(col1 - colh) > 2 || posh < 0 || posh > 63 || (ptab->getcasilla(posh) > 48 && ptab->getcasilla(posh) < 55))
                cout << "el movimiento 3 no es posible ";
            
            ///////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento es posible de realizar: //
            // 1. En la posici�n hipot�tica se encuentra un alfil.               //
            // 2. En la posici�n hipot�tica no hay otra ficha.                   //
            ///////////////////////////////////////////////////////////////////////
            
            else if((ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69) || ptab->getcasilla(posh) == 95){
                
                // Verifica si en la jugada se realiz� una captura
                if(ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69){
                    // Si se realiza una captura, se reinicia la variable "cont_com"
                    cont_com = 0;
                    (palfi + ptab->getcasilla(posh) - 65)->vivo = 0;
                }
                else
                    // Si no se realiza una captura, se suma +1 a la variable "cont_com"
                    cont_com++;
                
                /////////////////////////////////////////////////////////////////////////
                // Se realiza la jugada, asignando la ficha a la posici�n hipot�tica y //
                // borrando la ficha de su posici�n anterior.                          //
                // Al ser el movimiento posible, se cierra el switch.                  //
                /////////////////////////////////////////////////////////////////////////
                
                ptab->setcasilla(ficha, posh);
                ptab->setcasilla(95, pos);
                movyes = 1;
                movimiento_real_elegido = 3;
                cout << endl << "el movimiento 3 es posible " << endl;
                break;
            }
            
        case 4: // Movimiento 4 del
            posh = pos + 17; // Se calcula la posic�n hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            filh = posh / 8; // Se calcula la fila hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            colh = posh - (8*filh); // Se calcula la columna hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento no es posible de realizar:                               //
            // 1. Si la diferencia entre la fila en la que est� la ficha y la fila hipot�tica es mayor a 2.       //
            // 2. Si la diferencia entre la columna en la que est� la ficha y la columna hipot�tica es mayor a 2. //
            // 3. Si la posici�n hipot�tica es menor a 0 (esta no existe en el vector).                           //
            // 4. Si la posici�n hipot�tica es mayor a 63 (esta no existe en el vector).                          //
            // 5. Si en la posici�n hipot�tica se encuentra otro caballo.                                         //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            if(abs(fil1 - filh) > 2 || abs(col1 - colh) > 2 || posh < 0 || posh > 63 || (ptab->getcasilla(posh) > 48 && ptab->getcasilla(posh) < 55))
                cout << "el movimiento 4 no es posible ";
            
            ///////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento es posible de realizar: //
            // 1. En la posici�n hipot�tica se encuentra un alfil.               //
            // 2. En la posici�n hipot�tica no hay otra ficha.                   //
            ///////////////////////////////////////////////////////////////////////
            
            else if((ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69) || ptab->getcasilla(posh) == 95){
                
                // Verifica si en la jugada se realiz� una captura
                if(ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69){
                    // Si se realiza una captura, se reinicia la variable "cont_com"
                    cont_com = 0;
                    (palfi + ptab->getcasilla(posh) - 65)->vivo = 0;
                }
                else
                    // Si no se realiza una captura, se suma +1 a la variable "cont_com"
                    cont_com++;
                
                /////////////////////////////////////////////////////////////////////////
                // Se realiza la jugada, asignando la ficha a la posici�n hipot�tica y //
                // borrando la ficha de su posici�n anterior.                          //
                // Al ser el movimiento posible, se cierra el switch.                  //
                /////////////////////////////////////////////////////////////////////////
                
                ptab->setcasilla(ficha, posh);
                ptab->setcasilla(95, pos);
                movyes = 1;
                movimiento_real_elegido = 4;
                cout << endl << "el movimiento 4 es posible " << endl;
                break;
            }
            
        case 5: // Movimiento 5 del
            posh = pos + 15; // Se calcula la posic�n hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            filh = posh / 8; // Se calcula la fila hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            colh = posh - (8*filh); // Se calcula la columna hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento no es posible de realizar:                               //
            // 1. Si la diferencia entre la fila en la que est� la ficha y la fila hipot�tica es mayor a 2.       //
            // 2. Si la diferencia entre la columna en la que est� la ficha y la columna hipot�tica es mayor a 2. //
            // 3. Si la posici�n hipot�tica es menor a 0 (esta no existe en el vector).                           //
            // 4. Si la posici�n hipot�tica es mayor a 63 (esta no existe en el vector).                          //
            // 5. Si en la posici�n hipot�tica se encuentra otro caballo.                                         //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            if(abs(fil1 - filh) > 2 || abs(col1 - colh) > 2 || posh < 0 || posh > 63 || (ptab->getcasilla(posh) > 48 && ptab->getcasilla(posh) < 55))
                cout << "el movimiento 5 no es posible ";
            
            ///////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento es posible de realizar: //
            // 1. En la posici�n hipot�tica se encuentra un alfil.               //
            // 2. En la posici�n hipot�tica no hay otra ficha.                   //
            ///////////////////////////////////////////////////////////////////////
            
            else if((ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69) || ptab->getcasilla(posh) == 95){
                
                // Verifica si en la jugada se realiz� una captura
                if(ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69){
                    // Si se realiza una captura, se reinicia la variable "cont_com"
                    cont_com = 0;
                    (palfi + ptab->getcasilla(posh) - 65)->vivo = 0;
                }
                else
                    // Si no se realiza una captura, se suma +1 a la variable "cont_com"
                    cont_com++;
                
                /////////////////////////////////////////////////////////////////////////
                // Se realiza la jugada, asignando la ficha a la posici�n hipot�tica y //
                // borrando la ficha de su posici�n anterior.                          //
                // Al ser el movimiento posible, se cierra el switch.                  //
                /////////////////////////////////////////////////////////////////////////
                
                ptab->setcasilla(ficha, posh);
                ptab->setcasilla(95, pos);
                movyes = 1;
                movimiento_real_elegido = 5;
                cout << endl << "el movimiento 5 es posible " << endl;
                break;
            }
            
        case 6: // Movimiento 6 del
            posh = pos + 6; // Se calcula la posic�n hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            filh = posh / 8; // Se calcula la fila hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            colh = posh - (8*filh); // Se calcula la columna hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento no es posible de realizar:                               //
            // 1. Si la diferencia entre la fila en la que est� la ficha y la fila hipot�tica es mayor a 2.       //
            // 2. Si la diferencia entre la columna en la que est� la ficha y la columna hipot�tica es mayor a 2. //
            // 3. Si la posici�n hipot�tica es menor a 0 (esta no existe en el vector).                           //
            // 4. Si la posici�n hipot�tica es mayor a 63 (esta no existe en el vector).                          //
            // 5. Si en la posici�n hipot�tica se encuentra otro caballo.                                         //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            if(abs(fil1 - filh) > 2 || abs(col1 - colh) > 2 || posh < 0 || posh > 63 || (ptab->getcasilla(posh) > 48 && ptab->getcasilla(posh) < 55))
                cout << "el movimiento 6 no es posible ";
            
            ///////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento es posible de realizar: //
            // 1. En la posici�n hipot�tica se encuentra un alfil.               //
            // 2. En la posici�n hipot�tica no hay otra ficha.                   //
            ///////////////////////////////////////////////////////////////////////
            
            else if((ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69) || ptab->getcasilla(posh) == 95){
                
                // Verifica si en la jugada se realiz� una captura
                if(ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69){
                    // Si se realiza una captura, se reinicia la variable "cont_com"
                    cont_com = 0;
                    (palfi + ptab->getcasilla(posh) - 65)->vivo = 0;
                }
                else
                    // Si no se realiza una captura, se suma +1 a la variable "cont_com"
                    cont_com++;
                
                /////////////////////////////////////////////////////////////////////////
                // Se realiza la jugada, asignando la ficha a la posici�n hipot�tica y //
                // borrando la ficha de su posici�n anterior.                          //
                // Al ser el movimiento posible, se cierra el switch.                  //
                /////////////////////////////////////////////////////////////////////////
                
                ptab->setcasilla(ficha, posh);
                ptab->setcasilla(95, pos);
                movyes = 1;
                movimiento_real_elegido = 6;
                cout << endl << "el movimiento 6 es posible " << endl;
                break;
            }
            
        case 7: // Movimiento 7 del
            posh = pos - 10; // Se calcula la posic�n hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            filh = posh / 8; // Se calcula la fila hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            colh = posh - (8*filh); // Se calcula la columna hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento no es posible de realizar:                               //
            // 1. Si la diferencia entre la fila en la que est� la ficha y la fila hipot�tica es mayor a 2.       //
            // 2. Si la diferencia entre la columna en la que est� la ficha y la columna hipot�tica es mayor a 2. //
            // 3. Si la posici�n hipot�tica es menor a 0 (esta no existe en el vector).                           //
            // 4. Si la posici�n hipot�tica es mayor a 63 (esta no existe en el vector).                          //
            // 5. Si en la posici�n hipot�tica se encuentra otro caballo.                                         //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            if(abs(fil1 - filh) > 2 || abs(col1 - colh) > 2 || posh < 0 || posh > 63 || (ptab->getcasilla(posh) > 48 && ptab->getcasilla(posh) < 55))
                cout << "el movimiento 7 no es posible ";
            
            ///////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento es posible de realizar: //
            // 1. En la posici�n hipot�tica se encuentra un alfil.               //
            // 2. En la posici�n hipot�tica no hay otra ficha.                   //
            ///////////////////////////////////////////////////////////////////////
            
            else if((ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69) || ptab->getcasilla(posh) == 95){
                
                // Verifica si en la jugada se realiz� una captura
                if(ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69){
                    // Si se realiza una captura, se reinicia la variable "cont_com"
                    cont_com = 0;
                    (palfi + ptab->getcasilla(posh) - 65)->vivo = 0;
                }
                else
                    // Si no se realiza una captura, se suma +1 a la variable "cont_com"
                    cont_com++;
                /////////////////////////////////////////////////////////////////////////
                // Se realiza la jugada, asignando la ficha a la posici�n hipot�tica y //
                // borrando la ficha de su posici�n anterior.                          //
                // Al ser el movimiento posible, se cierra el switch.                  //
                /////////////////////////////////////////////////////////////////////////
                
                ptab->setcasilla(ficha, posh);
                ptab->setcasilla(95, pos);
                movyes = 1;
                movimiento_real_elegido = 7;
                cout << endl << "el movimiento 7 es posible " << endl;
                break;
            }
            
        case 8: // Movimiento 8 del
            posh = pos - 17; // Se calcula la posic�n hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            filh = posh / 8; // Se calcula la fila hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            colh = posh - (8*filh); // Se calcula la columna hipot�tica de la ficha, en caso de que este movimiento sea posible de realizar
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento no es posible de realizar:                               //
            // 1. Si la diferencia entre la fila en la que est� la ficha y la fila hipot�tica es mayor a 2.       //
            // 2. Si la diferencia entre la columna en la que est� la ficha y la columna hipot�tica es mayor a 2. //
            // 3. Si la posici�n hipot�tica es menor a 0 (esta no existe en el vector).                           //
            // 4. Si la posici�n hipot�tica es mayor a 63 (esta no existe en el vector).                          //
            // 5. Si en la posici�n hipot�tica se encuentra otro caballo.                                         //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            if(abs(fil1 - filh) > 2 || abs(col1 - colh) > 2 || posh < 0 || posh > 63 || (ptab->getcasilla(posh) > 48 && ptab->getcasilla(posh) < 55))
                cout << "el movimiento 8 no es posible ";
            
            ///////////////////////////////////////////////////////////////////////
            // Eval�a los casos en los que el movimiento es posible de realizar: //
            // 1. En la posici�n hipot�tica se encuentra un alfil.               //
            // 2. En la posici�n hipot�tica no hay otra ficha.                   //
            ///////////////////////////////////////////////////////////////////////
            
            else if((ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69) || ptab->getcasilla(posh) == 95){
                
                // Verifica si en la jugada se realiz� una captura
                if(ptab->getcasilla(posh) > 64 && ptab->getcasilla(posh) < 69){
                    // Si se realiza una captura, se reinicia la variable "cont_com"
                    cont_com = 0;
                    (palfi + ptab->getcasilla(posh) - 65)->vivo = 0;
                }
                else
                    // Si no se realiza una captura, se suma +1 a la variable "cont_com"
                    cont_com++;
                
                /////////////////////////////////////////////////////////////////////////
                // Se realiza la jugada, asignando la ficha a la posici�n hipot�tica y //
                // borrando la ficha de su posici�n anterior.                          //
                // Al ser el movimiento posible, se cierra el switch.                  //
                /////////////////////////////////////////////////////////////////////////
                
                ptab->setcasilla(ficha, posh);
                ptab->setcasilla(95, pos);
                movyes = 1;
                movimiento_real_elegido = 8;
                cout << endl << "el movimiento 8 es posible" << endl;
                break;
            }
            
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // Si se llega hasta esta parte del c�digo, significa que ninguno de los movimiento evaluados es //
            // posible de realizar. Se verifica que el primer movimiento que fue evaluado fuera diferente al //
            // movimiento 1 y se vuelve a llamar a la funci�n para evaluar aquellos movimientos que no han  //
            // sido verificados.                                                                             //
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            
            if(rand > 1)
                movyes = mov_pos_cab(pos,1,ficha,ptab);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // La funci�n devuelve un 1 o un 0. Esto indica si existe alg�n movimiento posible para esta ficha. Si //
    // devuelve un 0, significa que esta ficha en particular no se puede mover, por lo que se tendr� que   //
    // llamar a la siguiente ficha.                                                                        //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    cout << endl<<"El movimiento del caballo es: " << movimiento_real_elegido << endl;
    return movyes;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funci�n que verifica que el  seleccionado est� vivo, en ese caso llama a la funci�n "mov_pos_cab". //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void mov_cab(tablero *ptab){
    
    int random6, random8, exis = 0, ficha, pos;
    
    /////////////////////////////////////////////////////////////////////////////
    // Random6: n�mero aleatorio del caballo a llamar.                         //
    // Random8: n�mero aleatorio entre los 8 movimientos posibles del caballo. //
    // Exis: 0 => el caballo no existe, 1 => el caballo existe.                //
    // Ficha: c�digo ASCII correspondiente a la ficha.                         //
    // Pos: Posici�n del caballo con respecto al vector.                       //
    /////////////////////////////////////////////////////////////////////////////
    
    random6 = rand() % 6;
    ficha_opc_elegida = random6 + 1;
    
    while(exis == 0){ // Ciclo en el que se busca un  caballo
        
        for(int x = 0; x < 64; x++){ // Ciclo en el que se busca el caballo seleccionado por todo el vector
            if(ptab->getcasilla(x) == 49 + random6){
                // En caso de hallar el caballo, exis cambia a 1 y se acaba el ciclo while y se guardan la ficha y su posicion
                exis = 1;
                ficha = 49 + random6;
                pos = x;
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////////
        // Si el caballo no existe, el ciclo while contin�a pero buscando el siguiente            //
        // caballo. De la misma forma, si el caballo existe a�n as� se le sumar� 1 a la variable. //
        ////////////////////////////////////////////////////////////////////////////////////////////
        
        if(random6 == 5)
            // Si hemos llegado al �ltimo  y este no existe, se busca el primero de ellos
            random6 = 0;
        else
            // Si el caballo no existe, se busca el siguiente
            random6++;
    }
    
    random8 = (rand() % 8) + 1;
    movimiento_opc_elegido = random8;
    
    // Se llama a la funci�n "mov_pos_cab" para que verifique que el movimiento seleccionado es posible de realizar.
    int real = mov_pos_cab(pos, random8, ficha, ptab);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    // Se le asigna un 1 o un 0 a la variable real dependiendo el valor que devuelva la funci�n. Si //
    // devuelve un 0 significa que el caballo seleccionado no puede ejecutar ning�n movimiento, por //
    // lo que se tiene que buscar el siguiente caballo vivo.                                        //
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    exis = 0;
    while(real == 0){ // Ciclo en el que se busca un caballo seleccionado por todo el vector
        for(int x = 0; x < 64; x++){ // Ciclo en el que se busca un caballo seleccionado por todo el vector
            // En caso de hallar el caballo, exis cambia a 1 y se acaba el ciclo while y se guarda la ficha y su posicion
            if(ptab->getcasilla(x) == 49 + random6){
                exis = 1;
                ficha = 49 + random6;
                pos = x;
            }
        }
        
        if(exis == 1){
            // Se llama a la funci�n "mov_pos_cab" para que verifique que el movimiento seleccionado es posible de realizar.
            real = mov_pos_cab(pos, random8, ficha,ptab);
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////////
        // Si el caballo no existe, el ciclo while contin�a pero buscando el siguiente            //
        // caballo. De la misma forma, si el caballo existe a�n as� se le sumar� 1 a la variable. //
        ////////////////////////////////////////////////////////////////////////////////////////////
        
        if(random6 == 5)
            random6 = 0;
        else
            random6++;
    }
    if(random6  == 0){
        ficha_real_elegida = 6;
    }
    else
        ficha_real_elegida = random6;
    cout << endl<<"El caballo es: " << ficha_real_elegida << endl;
    
}



///////////////////////////////////////////////////////////////////////////////////////////
// Funci�n para verificar si el movimiento seleccionado del alfil es posible de realizar //
///////////////////////////////////////////////////////////////////////////////////////////

int mov_pos_alf(int pos, int rand, int ficha,tablero *ptab){
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Se traen la posici�n del alfil en t�rminos del vector (pos), un n�mero aleatorio que //
    // representa un movimiento (rand) y el alfil (ficha)                                   //
    //////////////////////////////////////////////////////////////////////////////////////////
    
    char c = ficha;
    int posh, col1, fil1, filmax, colmax, movyes = 0;
    fil1 = pos / 8; // Se calcula la fila de la ficha antes de realizar cualquier movimiento
    col1 = pos - (8*fil1); // Se calcula la columna de la ficha antes de realizar cualquier movimiento
    cout << endl <<"LA FICHA ES (c): " << c << endl; // Se muestra la ficha a mover
    
    
    ///////////////////////////////////////////////////////////////////////////////////////
    // El switch empieza a evaluar si el movimiento seleccionado es posible de realizar. //
    // Si este no es posible, recorre los siguientes movimientos hasta que alguno de     //
    // estos sea posible.                                                                //
    ///////////////////////////////////////////////////////////////////////////////////////
    
    switch(rand){
            
        case 1: // Movimiento 1 del alfil
            filmax = fil1; // Calcula la cantidad m�xima de filas que se puede mover el alfil
            colmax = 7 - col1; // Calcula la cantidad m�xima de columnas que se puede mover el alfil
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Verifica entre la fila m�xima y la columna m�xima cu�l de las dos es menor. La posici�n hipot�tica //
            // del alfil ser� la posici�n actual menos 7 veces la menor entre la fila m�xima y la columna m�xima. //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            if (filmax<=colmax)
                posh = pos - (7*filmax);
            else if (filmax>colmax)
                posh = pos - (7*colmax);
            
            // En caso de que la fila m�xima o la columna m�xima sean igual a 0, el movimiento no es posible.
            if(colmax == 0 || filmax == 0)
                cout << "Este movimiento 1 no es posible" << endl;
            
            /////////////////////////////////////////////////////////////////////////////////////////////////////
            // Sabiendo ya cual ser�a la posici�n final del alfil, se verifica que en las casillas intermedias //
            // entre la posici�n final y la posici�n actual no haya ning�n otro alfil o un caballo del equipo  //
            // contrario. Para esto se establece un ciclo que verifica cada una de estas casillas.             //
            /////////////////////////////////////////////////////////////////////////////////////////////////////
            
            else{
                for(int x = pos - 7; x>=posh; x-=7){
                    // Si no hay nada en esta casilla, el  se puede seguir moviendo
                    if(ptab->getcasilla(x) == 95){
                        ptab->setcasilla(ficha,x);
                        ptab->setcasilla(95,x + 7);
                        if(x == posh){
                            movyes = 1;
                            cont_com++;
                            break;
                        }
                    }
                    // Si hay un caballo en alguna de estas casillas, el alfil llega hasta esta posici�n y el ciclo se rompe.
                    else if(ptab->getcasilla(x) > 48 && ptab->getcasilla(x) < 55){
                        
                        (pcaba + ptab->getcasilla(x) - 49)->vivo = 0;
                        ptab->setcasilla(ficha,x);
                        ptab->setcasilla(95,x + 7);
                        movyes = 1;
                        cont_com = 0;
                        break;
                    }
                    else if(ptab->getcasilla(x) > 64 && ptab->getcasilla(x) < 69){
                        // Si hay un alfil en alguna de estas casillas, el afil queda un movimiento antes de llegar al otro .
                        if(x == pos-7){
                            // Si en el movimiento inmediatamente siguiente se llega a otro alfil, este movimiento no es posible
                            cout << "Este movimiento 1 no es posible" << endl;
                            break;
                        }
                        else{
                            movyes = 1;
                            cont_com++;
                            break;
                        }
                    }
                    
                    ////////////////////////////////////////////////////////////////////////////////////////////////
                    // Se mueve el alfil a la posici�n indicada por los condicionales anteriores, asign�ndole     //
                    // la ficha a esta posici�n y eliminando el alfil de la posici�n anterior. El ciclo se rompe. //
                    // Se verifica si hubo capturas en la jugada.                                                 //
                    ////////////////////////////////////////////////////////////////////////////////////////////////
                    
                    else if(x == posh && (ptab->getcasilla(x) == 95 || (ptab->getcasilla(x) > 48 && ptab->getcasilla(x) < 55))){
                        
                        if(ptab->getcasilla(x) == 95)
                            cont_com++;
                        
                        else
                            cont_com = 0;
                        
                        (pcaba + ptab->getcasilla(x) - 49)->vivo = 0;
                        ptab->setcasilla(ficha,x);
                        ptab->setcasilla(95,x + 7);
                        movyes = 1;
                        
                        break;
                    }
                }
            }
            
            if(movyes == 1){
                cout << endl << "El movimiento 1 es posible" << endl;
                movimiento_real_elegido = 1;
                return 1;
            }
            
        case 2: // Movimiento 2 del alfil
            filmax = 7 - fil1; // Calcula la cantidad m�xima de filas que se puede mover el alfil
            colmax = 7 - col1; // Calcula la cantidad m�xima de columnas que se puede mover el alfil
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Verifica entre la fila m�xima y la columna m�xima cu�l de las dos es menor. La posici�n hipot�tica //
            // del alfil ser� la posici�n actual menos 7 veces la menor entre la fila m�xima y la columna m�xima. //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            
            if (filmax<=colmax)
                posh = pos + (9*filmax);
            else if (filmax>colmax)
                posh = pos + (9*colmax);
            
            // En caso de que la fila m�xima o la columna m�xima sean igual a 0, el movimiento no es posible.
            if(colmax == 0 || filmax == 0)
                cout << "Este movimiento 2 no es posible" << endl;
            
            /////////////////////////////////////////////////////////////////////////////////////////////////////
            // Sabiendo ya cual ser�a la posici�n final del alfil, se verifica que en las casillas intermedias //
            // entre la posici�n final y la posici�n actual no haya ning�n otro alfil o un caballo del equipo  //
            // contrario. Para esto se establece un ciclo que verifica cada una de estas casillas.             //
            /////////////////////////////////////////////////////////////////////////////////////////////////////
            
            else{
                for(int x = pos + 9; x<=posh; x+=9){
                    // Si no hay nada en esta casilla, el alfil se puede seguir moviendo
                    if(ptab->getcasilla(x) == 95){
                        ptab->setcasilla(ficha,x);
                        ptab->setcasilla(95,x - 9);
                        if(x == posh){
                            movyes = 1;
                            cont_com++;
                            break;
                        }
                    }
                    // Si hay un caballo en alguna de estas casillas, el alfil llega hasta esta posici�n y el cilo se rompe.
                    else if(ptab->getcasilla(x) > 48 && ptab->getcasilla(x) < 55){
                        (pcaba + ptab->getcasilla(x) - 49)->vivo = 0;
                        ptab->setcasilla(ficha,x);
                        ptab->setcasilla(95,x - 9);
                        movyes = 1;
                        cont_com = 0;
                        break;
                    }
                    // Si hay un alfil en alguna de estas casillas, el afil queda un movimiento antes de llegar al otro .
                    else if(ptab->getcasilla(x) > 64 && ptab->getcasilla(x) < 69){
                        // Si en el movimiento inmediatamente siguiente se llega a otro alfil, este movimiento no es posible
                        if(x == pos+9){
                            cout << "Este movimiento 2 no es posible" << endl;
                            break;
                        }
                        else{
                            movyes = 1;
                            cont_com++;
                            break;
                        }
                    }
                    
                    ////////////////////////////////////////////////////////////////////////////////////////////////
                    // Se mueve el alfil a la posici�n indicada por los condicionales anteriores, asign�ndole     //
                    // la ficha a esta posici�n y eliminando el alfil de la posici�n anterior. El ciclo se rompe. //
                    // Se verifica si hubo capturas en la jugada.                                                 //
                    ////////////////////////////////////////////////////////////////////////////////////////////////
                    
                    else if(x == posh && (ptab->getcasilla(x) == 95 || (ptab->getcasilla(x) > 48 && ptab->getcasilla(x) < 55))){
                        if(ptab->getcasilla(x) == 95)
                            cont_com++;
                        
                        else
                            cont_com = 0;
                        (pcaba + ptab->getcasilla(x) - 49)->vivo = 0;
                        ptab->setcasilla(ficha,x);
                        ptab->setcasilla(95,x - 9);
                        movyes = 1;
                        break;
                    }
                }
            }
            
            if(movyes == 1){
                cout << endl << "El movimiento 2 es posible" << endl;
                movimiento_real_elegido = 2;
                return 1;
            }
            
        case 3: // Movimiento 3 del alfil
            filmax = 7 - fil1; // Calcula la cantidad m�xima de filas que se puede mover el alfil
            colmax = col1; // Calcula la cantidad m�xima de columnas que se puede mover el alfil
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Verifica entre la fila m�xima y la columna m�xima cu�l de las dos es menor. La posici�n hipot�tica //
            // del alfil ser� la posici�n actual menos 7 veces la menor entre la fila m�xima y la columna m�xima. //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            
            if (filmax<=colmax)
                posh = pos + (7*filmax);
            else if (filmax>colmax)
                posh = pos + (7*colmax);
            
            // En caso de que la fila m�xima o la columna m�xima sean igual a 0, el movimiento no es posible.
            if(colmax == 0 || filmax == 0)
                cout << "Este movimiento 3 no es posible" << endl;
            
            /////////////////////////////////////////////////////////////////////////////////////////////////////
            // Sabiendo ya cual ser�a la posici�n final del alfil, se verifica que en las casillas intermedias //
            // entre la posici�n final y la posici�n actual no haya ning�n otro alfil o un caballo del equipo  //
            // contrario. Para esto se establece un ciclo que verifica cada una de estas casillas.             //
            /////////////////////////////////////////////////////////////////////////////////////////////////////
            
            else{
                for(int x = pos + 7; x<=posh; x+=7){
                    // Si no hay nada en esta casilla, el alfil se puede seguir moviendo
                    if(ptab->getcasilla(x) == 95){
                        ptab->setcasilla(ficha,x);
                        ptab->setcasilla(95,x - 7);
                        if(x == posh){
                            movyes = 1;
                            cont_com++;
                            break;
                        }
                    }
                    // Si hay un caballo en alguna de estas casillas, el alfil llega hasta esta posici�n y el cilo se rompe.
                    else if(ptab->getcasilla(x) > 48 && ptab->getcasilla(x) < 55){
                        (pcaba + ptab->getcasilla(x) - 49)->vivo = 0;
                        ptab->setcasilla(ficha,x);
                        ptab->setcasilla(95,x - 7);
                        movyes = 1;
                        cont_com = 0;
                        break;
                    }
                    // Si hay un alfil en alguna de estas casillas, el afil queda un movimiento antes de llegar al otro .
                    else if(ptab->getcasilla(x) > 64 && ptab->getcasilla(x) < 69){
                        // Si en el movimiento inmediatamente siguiente se llega a otro alfil, este movimiento no es posible
                        if(x == pos+7){
                            cout << "Este movimiento 3 no es posible" << endl;
                            break;
                        }
                        else{
                            movyes = 1;
                            cont_com++;
                            break;
                        }
                    }
                    
                    
                    ////////////////////////////////////////////////////////////////////////////////////////////////
                    // Se mueve el alfil a la posici�n indicada por los condicionales anteriores, asign�ndole     //
                    // la ficha a esta posici�n y eliminando el alfil de la posici�n anterior. El ciclo se rompe. //
                    // Se verifica si hubo capturas en la jugada.                                                 //
                    ////////////////////////////////////////////////////////////////////////////////////////////////
                    
                    else if(x == posh && (ptab->getcasilla(x) == 95 || (ptab->getcasilla(x) > 48 && ptab->getcasilla(x) < 55))){
                        if(ptab->getcasilla(x) == 95)
                            cont_com++;
                        
                        else
                            cont_com = 0;
                        (pcaba + ptab->getcasilla(x) - 49)->vivo = 0;
                        ptab->setcasilla(ficha,x);
                        ptab->setcasilla(95,x - 7);
                        movyes = 1;
                        break;
                    }
                }
            }
            
            if(movyes == 1){
                cout << endl << "El movimiento 3 es posible" << endl;
                movimiento_real_elegido = 3;
                return 1;
            }
            
        case 4: // Movimiento 4 del alfil
            filmax = fil1; // Calcula la cantidad m�xima de filas que se puede mover el alfil
            colmax = col1; // Calcula la cantidad m�xima de columnas que se puede mover el alfil
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Verifica entre la fila m�xima y la columna m�xima cu�l de las dos es menor. La posici�n hipot�tica //
            // del alfil ser� la posici�n actual menos 7 veces la menor entre la fila m�xima y la columna m�xima. //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            if (filmax<=colmax)
                posh = pos - (9*filmax);
            else if (filmax>colmax)
                posh = pos - (9*colmax);
            
            // En caso de que la fila m�xima o la columna m�xima sean igual a 0, el movimiento no es posible.
            if(colmax == 0 || filmax == 0)
                cout << "Este movimiento 4 no es posible" << endl;
            
            /////////////////////////////////////////////////////////////////////////////////////////////////////
            // Sabiendo ya cual ser�a la posici�n final del alfil, se verifica que en las casillas intermedias //
            // entre la posici�n final y la posici�n actual no haya ning�n otro alfil o un caballo del equipo  //
            // contrario. Para esto se establece un ciclo que verifica cada una de estas casillas.             //
            /////////////////////////////////////////////////////////////////////////////////////////////////////
            
            else{
                for(int x = pos - 9; x>=posh; x-=9){
                    // Si no hay nada en esta casilla, el alfil se puede seguir moviendo
                    if(ptab->getcasilla(x) == 95){
                        ptab->setcasilla(ficha,x);
                        ptab->setcasilla(95,x + 9);
                        if(x == posh){
                            movyes = 1;
                            cont_com++;
                            break;
                        }
                    }
                    // Si hay un caballo en alguna de estas casillas, el alfil llega hasta esta posici�n y el cilo se rompe.
                    else if(ptab->getcasilla(x) > 48 && ptab->getcasilla(x) < 55){
                        (pcaba + ptab->getcasilla(x) - 49)->vivo = 0;
                        ptab->setcasilla(ficha,x);
                        ptab->setcasilla(95,x + 9);
                        movyes = 1;
                        cont_com = 0;
                        break;
                    }
                    // Si hay un alfil en alguna de estas casillas, el afil queda un movimiento antes de llegar al otro .
                    else if(ptab->getcasilla(x) > 64 && ptab->getcasilla(x) < 69){
                        // Si en el movimiento inmediatamente siguiente se llega a otro alfil, este movimiento no es posible
                        if(x == pos-9){
                            cout << "Este movimiento 4 no es posible" << endl;
                            break;
                        }
                        else{
                            movyes = 1;
                            cont_com++;
                            break;
                        }
                    }
                    
                    
                    ////////////////////////////////////////////////////////////////////////////////////////////////
                    // Se mueve el alfil a la posici�n indicada por los condicionales anteriores, asign�ndole     //
                    // la ficha a esta posici�n y eliminando el alfil de la posici�n anterior. El ciclo se rompe. //
                    // Se verifica si hubo capturas en la jugada.                                                 //
                    ////////////////////////////////////////////////////////////////////////////////////////////////
                    
                    else if(x == posh && (ptab->getcasilla(x) == 95 || (ptab->getcasilla(x) > 48 && ptab->getcasilla(x) < 55))){
                        if(ptab->getcasilla(x) == 95)
                            cont_com++;
                        
                        else
                            cont_com = 0;
                        (pcaba + ptab->getcasilla(x) - 49)->vivo = 0;
                        ptab->setcasilla(ficha,x);
                        ptab->setcasilla(95,x + 9);
                        movyes = 1;
                        break;
                    }
                }
            }
            
            if(movyes == 1){
                cout << endl << "El movimiento 4 es posible" << endl;
                movimiento_real_elegido = 4;
                return 1;
            }
            if(rand > 1)
                return mov_pos_alf(pos, 1, ficha,ptab);
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funci�n que verifica que el alfil seleccionado est� vivo, en ese caso llama a la funci�n "mov_pos_alf". //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void mov_alf(tablero *ptab){
    int random41,random42, exis = 0, ficha, pos;
    
    ////////////////////////////////////////////////////////////////////////////
    // Random41: n�mero aleatorio del alfil a llamar.                         //
    // Random42: n�mero aleatorio entre los 4 movimientos posibles del alfil. //
    // Exis: 0 => el alfil no existe, 1 => el alfil existe.                   //
    // Ficha: c�digo ASCII correspondiente a la ficha.                        //
    // Pos: Posici�n del alfil con respecto al vector.                        //
    ////////////////////////////////////////////////////////////////////////////
    
    random41 = rand() % 4;
    ficha_opc_elegida = random41 + 1;
    
    while(exis == 0){ // Ciclo en el que se busca un alfil
        
        for(int x = 0; x < 64; x++){ // Ciclo en el que se busca el alfil seleccionado por todo el vector
            if(ptab->getcasilla(x) == 65 + random41){
                // En caso de hallar el alfil, exis cambia a 1 y se acaba el ciclo while
                exis = 1;
                ficha = 65 + random41;
                // La posici�n del alfil es igual a su posici�n en el vector
                pos = x;
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        // Si el alfil no existe, el ciclo while contin�a pero buscando el siguiente          //
        // alfil. De la misma forma, si el alfil existe a�n as� se le sumar� 1 a la variable. //
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if(random41 == 3)
            // Si hemos llegado al �ltimo alfil y este no existe, se busca el primero de ellos
            random41 = 0;
        else
            // Si el alfil no existe, se busca el siguiente alfil
            random41++;
    }
    
    random42 = (rand() % 4) + 1;
    movimiento_opc_elegido = random42;
    
    // Se llama a la funci�n "mov_pos_alf" para que verifique que el movimiento seleccionado es posible de realizar.
    int real = mov_pos_alf(pos, random42, ficha,ptab);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    // Se le asigna un 1 o un 0 a la variable real dependiendo el valor que devuelva la funci�n. Si //
    // devuelve un 0 significa que el alfil seleccionado no puede ejecutar ning�n movimiento, por   //
    // lo que se tiene que buscar el siguiente alfil vivo.                                          //
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    exis = 0;
    while(real == 0) // Ciclo en el que se busca un alfil
    {
        for(int x = 0; x < 64; x++){ // Ciclo en el que se busca el alfil seleccionado por todo el vector
            if(ptab->getcasilla(x) == 65 + random41){
                // En caso de hallar el alfil, exis cambia a 1 y se acaba el ciclo while
                exis = 1;
                ficha = 65 + random41;
                // La posici�n del alfil es igual a su posici�n en el vector
                pos = x;
            }
        }
        
        if(exis == 1)
            // Se llama a la funci�n "mov_pos_alf" para que verifique que el movimiento seleccionado es posible de realizar.
            real = mov_pos_alf(pos, random42, ficha,ptab);
        
        ////////////////////////////////////////////////////////////////////////////////////////
        // Si el alfil no existe, el ciclo while contin�a pero buscando el siguiente          //
        // alfil. De la misma forma, si el alfil existe a�n as� se le sumar� 1 a la variable. //
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if(random41 == 3)
            random41 = 0;
        else
            random41++;
    }
    if(random41  == 0){
        ficha_real_elegida = 4;
    }
    else
        ficha_real_elegida = random41;
    
}

int leer_vector(int cont_fichas, tablero *ptab){
    int contcab=0,contalf=0;
    
    //Se hace el coneteo de caballos y alfiles en el vector con el fin de determinar si el juego ha terminado
    for(int x = 0; x < 64; x++){
        if(ptab->getcasilla(x) >= 49 && ptab->getcasilla(x) <= 54 ){
            if (alternancia==1)(((pcaba + ptab->getcasilla(x) - 49))->imagen).setPosition((int(x-int(x/8)*8)*100),(int((x/8)*100)));
            contcab++;
        }
        else if(ptab->getcasilla(x) >= 65 && ptab->getcasilla(x) <= 68 ){
            if(alternancia==1)(((palfi + ptab->getcasilla(x) - 65))->imagen).setPosition((int(x-int(x/8)*8)*100),(int((x/8)*100)));
            contalf++;
        }
    }
    
    //Se elije el mensaje por el cual el juego ha terminado
    if((contcab < 4 || contalf == 0 || cont_com == 10) && cont_fichas>9){
        ganador = 1;
        if(contalf == 0)
            return 1;
        else if(contcab < 4)
            return 2;
        else
            return 3;
    }
    return 0;
}

void Actualizar_dados(int fase){
    
    
    
    ultimox.setTexture(((dados)->pcarac)->textura);
    
    //Se asigna la texrura de las casillas opcional (aleatorio)y elejida (realmente se hizo)
    switch (fase){
        case 1:
            ultimoy.setTexture(((dados)->pcarac + ficha_opc_elegida)->textura);
            //ultimoy.setTexture((*(textura_dado + ficha_opc_elegida)));
            ultimoy.setPosition(1100,75);
            ultimox.setPosition(900,75);
            break;
        case 2:
            ultimoy.setTexture(((dados)->pcarac + movimiento_opc_elegido)->textura);
            //ultimoy.setTexture((*(textura_dado + movimiento_opc_elegido)));
            ultimoy.setPosition(1100,220);
            ultimox.setPosition(900,220);
            break;
        case 3:
            ultimoy.setTexture(((dados)->pcarac + ficha_real_elegida)->textura);
            //ultimoy.setTexture((*(textura_dado + ficha_real_elegida)));
            ultimoy.setPosition(1100,510);
            ultimox.setPosition(900,510);
            break;
        case 4:
            ultimoy.setTexture(((dados)->pcarac + movimiento_real_elegido)->textura);
            //ultimoy.setTexture((*(textura_dado + movimiento_real_elegido)));
            ultimoy.setPosition(1100,670);
            ultimox.setPosition(900,670);
            break;
    }
}

struct NODO *tablero::obt_pos(int n){
    struct NODO *aux1, *aux2;
    aux1 = cabeza;
    aux2 = cabeza->sig;
    while(aux1->num != n){
        aux1 = aux2;
        aux2 = aux2->sig;
    }
    return aux1;
}

char tablero::getcasilla(int n){
    return (obt_pos(n))->casilla;
}

tablero::~tablero(){
    struct NODO *p;
    int a = 1;
    p=cabeza;
    while (cabeza != NULL){
        cabeza=cabeza->sig;
        delete p;
        cout << " Nodo "<< a <<" destruido..." << endl;
        a++;
        p=cabeza;
    }
}
