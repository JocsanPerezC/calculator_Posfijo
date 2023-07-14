// CALCULADORA POSFIJA
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <limits.h>
#include <iostream>


using namespace std;
//******************************************

//Nodo pricipal que se utilizara para todas las operaciones.
typedef struct Nodo
{
	char simbol;  // Este es el char que en caso de ser un nodo operador se tomara en cuenta para el analisis.
	int valor;  // Este es el valor que se utiliza para cuando se trabaje con numeros, o se tenga que evaluar la prioridad en caso de ser un nodo operador.
	bool esOperador; // Este booleano lo utilizamos para saber si el nodo actua como un operardor o como un valor o simbolo deferente de estos.
	float operacion; //Debido a la presencia de divisiones se  utilizara este parametro en la evaluacion de resultados.
	Nodo* Siguiente; //Puntero del mismo tipo para la lista enlazada.

}*PtrNodo;

//******************************************


//Pilas que se utilizaran en los diversos algoritmos, siendo la de operadores, la de salidad de expresiones y la que se utilizara para evaluar las instrucciones.
PtrNodo PilaOp;
PtrNodo PilaSalida;
PtrNodo PilaEval;

void IniciarPila(PtrNodo& nodo) //Se inizializa cualquier pila pasada.
{
	nodo = NULL;
}

void DestruirPila(PtrNodo& nodo) //Se destruye completamente cualquier lista enlazada (pila) pasada.
{
	PtrNodo Aux;
	Aux = nodo;
	while (Aux != NULL)
	{
		nodo = nodo->Siguiente;
		delete(Aux);
		Aux = nodo;
	}
}

bool esNum(char num) { //Esta devuelve true si el char pasado corresponde a un numero, segun la tabla ASCII, false de lo contrario.
	if (num == 48 || num == 49 || num == 50 || num == 51 || num == 52 || num == 53 || num == 54 || num == 55 || num == 56 || num == 57) {
		return true;
	}
	else {
		return false;
	}
}

bool esOpe(char sim) { //Esta devuelve true si el char pasado corresponde a un operador de la calculadora, segun la tabla ASCII, false de lo contrario.
	if (sim == '+' || sim == '-' || sim == '/' || sim == '*' || sim == '(' || sim == ')') {
		return true;
	}
	else {
		return false;
	}
}

/*Toma una pila(lista enlzada), y devuelve como un string con espacios entre sus simbolos, esto se utilizara a la hora de convertir una
expresion infija a una posfija. Esta se utiliza cuando la pila no tiene que tomar en cuenta valores numericos.*/
string ListaEnlazadaToString(PtrNodo nodo) {
	PtrNodo Aux;
	string str2 = ""; //String que contendra el resultado.
	string strA = ""; //String auxiliar para acomudar el string de manera correcta.
	Aux = nodo;
	while (Aux != NULL)
	{
		strA += Aux->simbol + (" " + str2); //Al nuevo simbolo a agregar se le suma un espacio y el previo srt2, permitiendo insertar el simbolo al inicio del string auxiliar.
		str2 = strA; // Se iguala para que la respuesta este en el string de salida.
		strA = ""; //Se limpia el string auxiliar.
		Aux = Aux->Siguiente;
	}
	return str2; //Se retorna el resultado.
}

/*Toma una pila(lista enlzada), y devuelve como un string con espacios entre sus componentes, esto se utilizara a la hora de convertir una
expresion infija a una posfija y evaluar el valor de sus variables. Esta se utiliza tomando en cuenta que los nodos que no sean operadores se agragara su valor*/
string ListaEnlazadaToString2(PtrNodo nodo) {
	PtrNodo Aux;
	string str2 = "";
	string strA = "";
	Aux = nodo;
	//Se sigue el mismo sistema de la funcion anterior, solo que en caso de no ser un nodo operador, lo que se agrega es el valor del nodo.
	while (Aux != NULL)
	{
		if (Aux->esOperador) {
			strA += Aux->simbol + (" " + str2);
			str2 = strA;
			strA = "";
		}
		else {
			strA += to_string(Aux->valor) + " " + str2;
			str2 = strA;
			strA = "";
		}
		Aux = Aux->Siguiente;
	}
	return str2; //devuelve como un string con espacios entre sus componentes
}

PtrNodo CrearNodo(char sim, int val)// Crea y retorna un nodo, donde si es operador o no se decidira en base a char pasado.
{
	PtrNodo nodo = new(Nodo); //Crea el nodo.
	if (esOpe(sim)) {
		//Si el char pasado es uno de operador, se asigna este a simbolo, se declara esOperador como true y se le asigan en valor la proriedad dependiendo del char.
		nodo->simbol = sim;
		nodo->esOperador = true;
		switch (sim) {
		case '+':
			nodo->valor = 1;
			break;
		case '-':
			nodo->valor = 1;
			break;
		case '/':
			nodo->valor = 2;
			break;
		case '*':
			nodo->valor = 2;
			break;
		case '(':
			nodo->valor = 0;
			break;
		}
	}
	else {
		//Si no es un operardor se igualan los valores pasados y se declara esOperador como false.
		nodo->simbol = sim;
		nodo->valor = val;
		nodo->esOperador = false;
	}
	//Se asignan los valores que no dependen del tipo y se retorna.
	nodo->Siguiente = NULL;
	nodo->operacion = 0;
	return nodo;
}

void AgregarInicio(PtrNodo& Lista, PtrNodo& Nuevo) //Funcion que recibiendo dos referencias una de una lista y una de un nuevo objeto, agrega al inicio de la lista el objeto nuevo.
{
	Nuevo->Siguiente = Lista;
	Lista = Nuevo;
}


void Push(PtrNodo& nodo, PtrNodo Nuevo) { //Dada una pila y un objeto, hace un agregar al incio.
	AgregarInicio(nodo, Nuevo);
}

PtrNodo Top(PtrNodo& nodo) { //Dada una pila, retorna, sin desligar, el primer nodo de la pila, osea el tope de la misma.  
	return nodo;
}

PtrNodo Pop(PtrNodo& nodo) { //Dada una pila, retorna, desligando, el primer nodo de la pila. 
	PtrNodo Aux = nodo;
	nodo = nodo->Siguiente;
	Aux->Siguiente = NULL;
	return Aux;
}





/*Esta funcion toma un string asumido en infijo y retorna un string que represente la misma formula pero en formato posfijo.*/
string convertirInPos(string str) {
	bool aux;
	PtrNodo Nuevo; //Puntero auxiliar de crecio.
	PtrNodo Auxi; //Puntero auxiliar.
	for (int i = 0; i < str.size(); i++) { //Se recorre el string paso por paso.
		aux = true; //Auxiliar para los ciclos.
		if (esOpe(str[i])) { //Si el valor es un operando.
			if (str[i] == ')') { //En caso de ser el simbolo ")", significara que se debe sacar los que haya en la pila de operadores y colocarlo en la de salida hasta encontrar un "(".
				while (aux) {
					Auxi = Pop(PilaOp);//Si no se ha encontrado un "(" se la hace pop a la pila de operadores, y se coloca en el puntero auxiliar.
					if (Auxi->simbol == '(') { //Si el nodo sacado de la pila tiene simbolo "(", se elimana el nodo y se termina el ciclo de vaciado.
						delete(Auxi);
						aux = false;
					}
					else {//De lo contrario hacerle un push al nodo en la pila de salida.
						Push(PilaSalida, Auxi);
					}
				}

			}
			else {//Si el operador no es ")".
				Nuevo = CrearNodo(str[i], 0); //Se crea un nodo con el simbolo actual y valor 0.
				while (aux) {//Se genera un ciclo, ya que dependiendo si el operador en el tope de la pila tiene un prioridad mayor o igual que el actual se debe sacar el mismo de la pila de operadores y meterlo a la pila de salida, epitiendo el proceso hasta que se cumpla lo anterior. 
					if (PilaOp == NULL || Top(PilaOp)->valor < Nuevo->valor || Nuevo->simbol == '(') { //Si la pila de operadores esta vacia, la prioridad del top es menor que del nuevo nodo o el nuevo simbolo es un "(".
						//Se agrega el nuevo nodo a la pila de operadores, y se termina el ciclo.
						Push(PilaOp, Nuevo);
						aux = false;
					}
					else { //Si alguna de las opciones no se cumple.
						//Se sacar el top de la pila de operadores y colocarlo en la pila de salida.
						Auxi = Pop(PilaOp);
						Push(PilaSalida, Auxi);
					}
				}
			}
		}
		else if (str[i] != ' ') { //Si el simbolo evaluado es un espacio, este se genera en un nodo y se agrega a la lista de salida.
			Nuevo = CrearNodo(str[i], -1);
			Push(PilaSalida, Nuevo);
		}
	}
	while (PilaOp != NULL) { //Al terminar se verifica que la pila de operadores este vacia, donde en caso contrario se sacan su contenido para colocarlo en la lista de salida.
		Auxi = Pop(PilaOp);
		Push(PilaSalida, Auxi);
	}
	Auxi = NULL;
	Nuevo = NULL;
	return ListaEnlazadaToString(PilaSalida); //Finalmente se retorna la lista de salida convertida en un string.
}

/*Esta funcion toma un string asumido en posfijo y evalua sus valores, para devolver un valor de resulatdo.*/
float EvaluacionPosfija(string str) {
	float res2 = 0; //Auxiliar de resultado.
	string str2 = ""; //Auxiliar para convertir string a nuemro.
	//Punteros a nodos para crea nodos o auxiliaras para su traslado.
	PtrNodo Nuevo;
	PtrNodo Auxi1;
	PtrNodo Auxi2;
	for (int i = 0; i < str.size(); i++) { //Se recorre el string paso por paso.
		if (esOpe(str[i])) {// Si es un operador se le hace pop dos veces a la pila de evaluacion, sacando los dos valores de mas arriba en la pila.
			Auxi1 = Pop(PilaEval);
			Auxi2 = Pop(PilaEval);
			switch (str[i]) {// Se pregunta que tipo de operador son, donde en cada caso se realiza la operacion correspondiente y se guarda en res2. Aui se utiliza el atributo float operacion.
			case '+':
				res2 = (float)Auxi2->operacion + Auxi1->operacion;
				break;
			case '-':
				res2 = (float)Auxi2->operacion - Auxi1->operacion;
				break;
			case '/':
				if (Auxi1->operacion == 0) {//Si se genera una division por 0, se retorna error y se devuelve los nodos a la pila de evaluacion.
					cout << "Error division por 0" << endl;
					system("Pause");
					Push(PilaEval, Auxi2);
					Push(PilaEval, Auxi1);
					return NULL;
				}
				else {
					res2 = (float)Auxi2->operacion / Auxi1->operacion;
				}
				break;
			case '*':
				res2 = (float)Auxi2->operacion * Auxi1->operacion;
				break;
			}
			//Finalmente se genera un nuevo nodo que no es un operador, se le asigna el valor de res2 en su parametro de operaciones y se añade a la pila de evaluacion.
			Nuevo = CrearNodo('#', 0);
			Nuevo->operacion = res2;
			Push(PilaEval, Nuevo);
			//Finalmente se elimianan los operandos.
			delete(Auxi2);
			delete(Auxi1);
			Auxi1 = NULL;
			Auxi2 = NULL;

		}
		else if (esNum(str[i])) { //En caso de se un numero el simbolo evaluado.
			str2 += str[i]; //Este se agregara al str2.
			if ((str[i + 1] != NULL && !esNum(str[i + 1])) || str[i + 1] == NULL) { //Si no se ha llegado al simbolo final y el siguiente valor no es un numero, o si se ha llegado al final.
				//Se convierte el string guardado en str2 a un valor, donde se crea un nodo y se agrea a la pila de avaluacion, ademas de limpiar al str2.
				int number = stoi(str2);
				Nuevo = CrearNodo('#', number);
				Nuevo->operacion = number;
				Push(PilaEval, Nuevo);
				str2 = "";
			}
		}
	}
	Nuevo = NULL;
	return Top(PilaEval)->operacion; //Finalmente se retorna el top de la pila de evaluacion el cual contendra el resultado de la evaluacion.
}

/*Esta funcion recibe una pila, y donde los nodos que no sean operadores pide por consola un valor para asignarles.*/
void captarValores(PtrNodo& nodo) {
	PtrNodo Aux;
	int num;
	Aux = nodo;
	while (Aux != NULL)
	{
		if (!Aux->esOperador) {//Si el nodo no es operador.
			//Pide el valor y se lo asigna al nodo.
			cout << endl << "Ingrese el valor de la variable " << Aux->simbol << ": ";
			cin >> num;
			Aux->valor = num;
		}
		Aux = Aux->Siguiente;
	}
}



void main(int argc, char* argv[])
{
	//************************************************************************

	float resultado; //Auxiliar para resultado.
	//Auxiliares para tratar con lso string con las expresiones
	string str3;
	string str;
	string str2;
	string str1;


	//************************************************************************	

	//Se inicializan las pilas.
	IniciarPila(PilaOp);
	IniciarPila(PilaSalida);
	IniciarPila(PilaEval);

	int opc;//Selector de opciones
	bool menu = true; //Menu del programa.
	if (argc == 1) {//Se la contidad de algomentos del programa es solo 1, entonces significa que este se abrio desde el .exe a una consola de comandos.
		while (menu) {
			try {
				//Menu.
				system("CLS");
				cout << " ********* Calculadora *********" << endl << endl;
				cout << "  1. Convertir de Infijo A Posfijo" << endl;
				cout << "  2. Evaluar una expresion Posfija" << endl;
				cout << "  3. Conversion y evaluacion de expresion." << endl;
				cout << "  4. Salir" << endl;
				cout << endl << "	Digite la opcion: ";
				cin >> opc;//Se toma la opcion digitada.
				switch (opc) {
				case 1: { // En caso de ser la primera, se solicita el string infijo, se convierte con la funcion correspondiente y se imprime. Ademas de destruir las pilas utilizadas.
					system("CLS");
					cout << "---#Conversion de Infijo A Posfijo#---" << endl;
					cout << "Ingrese la expresion Infija: ";
					cin >> str;
					str3 = convertirInPos(str);
					cout << endl << "Expresion Posfija: " << str3 << endl;
					system("Pause");
					DestruirPila(PilaOp);
					DestruirPila(PilaSalida);
					break;

				}
				case 2: { // En caso de ser la primera, se solicita el string posfijo con valores numericos, se evalua con la funcion correspondiente y se imprime. Ademas de destruir la pila utilizada.
					system("CLS");
					cout << "---#Evaluacion de Posfijo#---" << endl;
					cout << "Ingrese la expresion Posfija: ";
					cin >> str2;
					resultado = EvaluacionPosfija(str2);
					system("Pause");
					cout << endl << "Resultado de la expresion: " << resultado << endl;
					system("Pause");
					DestruirPila(PilaEval);
					break;
				}
				case 3: {
					//Para esta opcion se realiza la peticion, coversion e impresion de la expresion infija.
					system("CLS");
					cout << "---#Conversion y evaluacion#---" << endl;
					cout << "Ingrese la expresion Infija: ";
					cin >> str;
					str3 = convertirInPos(str);
					cout << endl << "Expresion resultante: " << str3 << endl;
					system("Pause");
					//Despues se toma la pila de salida y se capturan los valores de las variables dentro de la misma.
					captarValores(PilaSalida);
					//Ya con los valores, se convierte la pila de salida a un string.
					str1 = ListaEnlazadaToString2(PilaSalida);
					//Y se evalua para dar el resulado, imprimirlo y destruir las pilas usadas.
					resultado = EvaluacionPosfija(str1);
					cout << endl << "Resultado de la expresion: " << resultado << endl;
					system("Pause");
					DestruirPila(PilaOp);
					DestruirPila(PilaSalida);
					DestruirPila(PilaEval);
					break;
				}
				case 4: { // Se sale dal programa terminando el ciclo.
					menu = false;
					system("CLS");
					cout << "Saliendo..." << endl;
					cout << "Gracias por preferir nuestros servicios" << endl;
					system("Pause");
					break;
				}
				}
			}
			catch (exception e) {
				system("CLS");
				cout << "Error!!" << endl;
				system("Pause");
				menu = false;
			}
		}
	}
	else if (argc == 3) { //Si se reciben tres argumentos significa que se llamo el .exe desde el cmd, siendo el argumento dos la funcion y el 3 la expresion.
		string strN = argv[1]; //Se guarda el argumento 2 para conmpararlo.
		string strM = argv[2];//Se guarda el argumento 3 para trabajar con el.
		if (strN == "convert_a_posfijo") { //Si el argumento 2 es igual a "convert_a_posfijo", entonces se toma el argumento tres, se convierte a posfijo y se imprime en la consola.
			str3 = convertirInPos(strM);
			cout << endl << "Expresion Posfija: " << str3 << endl;
			system("Pause");
		}
		else if (strN == "eval_el_posfijo") { //Si el argumento 2 es igual a "eval_el_posfijo", entonces se toma el argumento tres, se evalua y se imprime en la consola.
			resultado = EvaluacionPosfija(strM);
			cout << endl << "Resultado de la expresion: " << resultado << endl;
			system("Pause");
		}
		else { //Si el argumento dos no coincide se devulve por consola error.
			system("CLS");
			cout << "Error: Funcion incorrecta" << endl;
			system("Pause");
		}
	}
	else { //Si el programa recibe un numero de argumentos diferentes de 1 o 3, devuelve error.
		system("CLS");
		cout << "Error: Numero de valores incorrecto" << endl;
		system("Pause");
	}



}