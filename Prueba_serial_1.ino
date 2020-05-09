/*  file: prueba_serial_1.ino
 *  author: riizdo 
 *  date: 19/04/2020
 *  description: comunication test
 */

//declaration of types////////////////////////////////////////////////////
typedef enum {NO_ERROR, MEMORY_ERROR} errorType;

typedef struct {
  int nParameter;
  int *parameter;
} parameterListType;

//declaration of variables/////////////////////////////////////////////////
parameterListType parameterList;
errorType error;

//declaration of headers///////////////////////////////////////////////////
void parameter_init(parameterListType *list);
errorType parameter_read_serial(parameterListType *list);
errorType parameter_add(parameterListType *list, int element);
void parameter_print(parameterListType list);
void parameter_send_serial(parameterListType list);

//function setup///////////////////////////////////////////////////////////********************************************
void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);

  while (!Serial1) {
    Serial.println("esperando a serial");
  }

  //init the variables///////////////////////////////
  parameter_init(&parameterList);
  error = NO_ERROR;
}//close setup

//function loop/////////////////////////////////////////////////////////////*******************************************
void loop() {
  if (Serial1.available() > 0) {
    error = parameter_read_serial(&parameterList);
    parameter_print(parameterList);
    Serial.println(error);
  }
  
}//close loop

//function parameter init///////////////////////////////////////////////////
void parameter_init(parameterListType *list) {
  if (list->parameter != NULL) {
    free(list->parameter);
  }
  list->nParameter = 0;
}

//function read serial parameter////////////////////////////////////////////
errorType parameter_read_serial(parameterListType *list) {
  int nData = Serial1.available();
  if (nData > 0) {
    if (list->nParameter == 0) {
      list->parameter = (int*) malloc(nData * sizeof(int));
    } else {
      list->parameter = (int*) realloc(list->parameter, nData * sizeof(int));
    }
    if (list->parameter != NULL) {
      for (int i = 0; i < nData; i++) {
        list->parameter[i] = Serial1.read();
      }
    } else {
      return MEMORY_ERROR;
    }
  }

  return NO_ERROR;
}

//function parameter add///////////////////////////////////////////////////
errorType parameter_add(parameterListType *list, int element) {
  if (list->nParameter == 0) {
    list->parameter = (int*) malloc(sizeof(int));
  } else {
    list->parameter = (int*) realloc(list->parameter, (list->nParameter + 1) * sizeof(int));
  }

  if (list->parameter == NULL) {
    return MEMORY_ERROR;
  }

  list->parameter[list->nParameter] = element;
  list->nParameter++;

  return NO_ERROR;
}

//function parameter print//////////////////////////////////////////////////
void parameter_print(parameterListType list) {
  for (int i = 0; i < list.nParameter; i++) {
    Serial.println(list.parameter[i]);
  }
}

//function send serial parameter////////////////////////////////////////////
void parameter_send_serial(parameterListType list) {
  for (int i = 0; i < list.nParameter; i++) {
    Serial1.write(list.parameter[i]);
  }
}
