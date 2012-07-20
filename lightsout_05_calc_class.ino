#define MAXNUMSTACK 32
#define MAXOPSTACK 32
#define MAXEXPRSIZE 128
#define LCDCOLS 8

class Calc {
  public:
  double numstack[MAXNUMSTACK];
  int numstack_size;
  char expression[MAXEXPRSIZE];
  int expr_size;
  char buffer[16];
  int eval;
  double result;
  char c;
  int window_start;
  int window_end;
  int button;

  enum {ASSOC_NONE=0, ASSOC_LEFT, ASSOC_RIGHT};

  struct operator_type {
    char op;
    int prec;
    int assoc;
    int unary;
    double (*eval)(double a1, double a2);
  };

  operators[]={
    {'_', 10, ASSOC_RIGHT, 1, eval_uminus},
    {'^', 9,  ASSOC_RIGHT, 0, eval_exp},
    {'*', 8,  ASSOC_LEFT,  0, eval_mul},
    {'/', 8,  ASSOC_LEFT,  0, eval_div},
    {'%', 8,  ASSOC_LEFT,  0, eval_mod},
    {'+', 5,  ASSOC_LEFT,  0, eval_add},
    {'-', 5,  ASSOC_LEFT,  0, eval_sub},
    {'(', 0,  ASSOC_NONE,  0, NULL},
    {')', 0,  ASSOC_NONE,  0, NULL}
  };

  struct operator_type *opstack[MAXOPSTACK];
  int nopstack;

  Calc() {
    numstack_size = 0;
    expr_size     = 0;
    eval          = 0;
    result        = 0;
    nopstack      = 0;
  }

  void clear_display() {
    max_print_progmem(string_empty,0,0);
    max_print_progmem(string_empty,1,0);
  }

  char get_keypress() {
    switch(button) {
      case 15:
        return '7';
        break;
      case 14:
        return '8';
        break;
      case 13:
        return '9';
        break;
      case 12:
        return '/';
        break;

      case 11:
        return '4';
        break;
      case 10:
        return '5';
        break;
      case 9:
        return '6';
        break;
      case 8:
        return '*';
        break;

      case 7:
        return '1';
        break;
      case 6:
        return '2';
        break;
      case 5:
        return '3';
        break;
      case 4:
        return '-';
        break;

      case 3:
        return '0';
        break;
      case 2:
        return '.';
        break;
      case 18:
        return 127;
        break;
      case 1:
        return 13;
        break;
      case 0:
        return '+';
        break;
    }
    return 0;
  }

  void clear_stack(){
    for (int i=0; i<MAXNUMSTACK; i++){
      numstack[i]=0;
    }
    numstack_size=0;
  }

  double eval_add(double a1, double a2) { return a1+a2; }
  double eval_sub(double a1, double a2) { return a1-a2; }
  double eval_uminus(double a1, double a2) { return -a1; }
  double eval_exp(double a1, double a2) { return a2<0 ? 0 : (a2==0?1:a1*eval_exp(a1, a2-1)); }
  double eval_mul(double a1, double a2) { return a1*a2; }
  double eval_div(double a1, double a2) {
    if(!a2) {
      Serial.println("ERROR: Division by zero\n");
      return NULL;
    }
    return a1/a2;
  }
  double eval_mod(double a1, double a2) {
    if(!a2) {
      Serial.println("ERROR: Division by zero\n");
      return NULL;
    }
    return fmod(a1, a2);
  }

  struct operator_type *getop(char ch) {
    int i;
    for(i=0; i<sizeof operators/sizeof operators[0]; ++i) {
      if(operators[i].op==ch) return operators+i;
    }
    return NULL;
  }

  void push_opstack(struct operator_type *op) {
    if(nopstack>MAXOPSTACK-1) {
      Serial.println("ERROR: Operator stack overflow\n");
      exit(1);
    }
    opstack[nopstack++]=op;
  }

  struct operator_type *pop_opstack() {
    if(!nopstack) {
      Serial.println("ERROR: Operator stack empty\n");
      exit(1);
    }
    return opstack[--nopstack];
  }

  void push_numstack(double num) {
    if(numstack_size>MAXNUMSTACK-1) {
      Serial.println("ERROR: Number stack overflow\n");
      exit(1);
    }
    numstack[numstack_size++]=num;
  }

  double pop_numstack() {
    if(!numstack_size) {
      Serial.println("ERROR: Number stack empty\n");
      exit(1);
    }
    return numstack[--numstack_size];
  }

  void shunt_op(struct operator_type *op) {
    struct operator_type *pop;
    double n1, n2;

    if(op->op=='(') {
      push_opstack(op);
      return;

    } else if(op->op==')') {
      while(nopstack>0 && opstack[nopstack-1]->op!='(') {
        pop=pop_opstack();
        n1=pop_numstack();

        if(pop->unary) push_numstack(pop->eval(n1, 0));
        else {
          n2=pop_numstack();
          push_numstack(pop->eval(n2, n1));
        }
      }

      if(!(pop=pop_opstack()) || pop->op!='(') {
        Serial.println("ERROR: Stack error. No matching \'(\'\n");
        exit(1);
      }
      return;
    }

    if(op->assoc==ASSOC_RIGHT) {
      while(nopstack && op->prec<opstack[nopstack-1]->prec) {
        pop=pop_opstack();
        n1=pop_numstack();
        if(pop->unary) push_numstack(pop->eval(n1, 0));
        else {
          n2=pop_numstack();
          push_numstack(pop->eval(n2, n1));
        }
      }
    } else {
      while(nopstack && op->prec<=opstack[nopstack-1]->prec) {
        pop=pop_opstack();
        n1=pop_numstack();
        if(pop->unary) push_numstack(pop->eval(n1, 0));
        else {
          n2=pop_numstack();
          push_numstack(pop->eval(n2, n1));
        }
      }
    }
    push_opstack(op);
  }

  int isdigit_or_decimal(int c) {
    if (c == '.' || isdigit(c))
      return 1;
    else
      return 0;
  }

  double eval_expression() {
    char *expr;
    char *tstart=NULL;
    struct operator_type startop={'X', 0, ASSOC_NONE, 0, NULL};  /* Dummy operator to mark start */
    struct operator_type *op=NULL;
    double n1, n2;
    struct operator_type *lastop=&startop;

    for (expr=expression; *expr; ++expr) {
      if (!tstart) {

        if ((op=getop(*expr))) {

          if (lastop && (lastop == &startop || lastop->op != ')')) {
            if (op->op == '-')
              op=getop('_');
            else if (op->op!='(') {
              Serial.print("ERROR: Illegal use of binary operator ");
              Serial.println(op->op);
              return NULL;
            }
          }
          shunt_op(op);
          lastop=op;
        } else if (isdigit_or_decimal(*expr)) tstart = expr;
        else if (!isspace(*expr)) {
          Serial.print("ERROR: Syntax error\n");
          return NULL;
        }
      } else {
        if (isspace(*expr)) {
          push_numstack(atof(tstart));
          tstart=NULL;
          lastop=NULL;
        } else if ((op=getop(*expr))) {
          push_numstack(atof(tstart));
          tstart=NULL;
          shunt_op(op);
          lastop=op;
        } else if (!isdigit_or_decimal(*expr) ) {
          Serial.print("ERROR: Syntax error\n");
          return NULL;
        }
      }
    }
    if(tstart) push_numstack(atof(tstart));

    while(nopstack) {
      op=pop_opstack();
      n1=pop_numstack();
      if(op->unary) push_numstack(op->eval(n1, 0));
      else {
        n2=pop_numstack();
        push_numstack(op->eval(n2, n1));
      }
    }

    if(numstack_size!=1) {
      Serial.print("ERROR: Number stack has ");
      Serial.print(numstack_size);
      Serial.print(" elements after evaluation. Should be 1.\n");
      clear_stack();
      return NULL;
    }
    Serial.println(numstack[0]);
    return numstack[0];
  }

  void begin() {
    while (1) {
      button = read_buttons();
      if (button >= 0) { // there is a button press
        c = get_keypress();

        if (c == 127) { // backspace
          expression[expr_size-1]= ' ';
          expr_size--;
        }
        else if (c == 13) { // enter
          eval = 1;
        }
        else if (c == 27) { // esc
          for (int i=0; i<EXPRSIZE; i++) {
            expression[i] = ' ';
          }
          expression[EXPRSIZE-1]='\0';
          expr_size = 0;
          clear_display();
        }
        else if (expr_size < EXPRSIZE-1) {
          expression[expr_size] = c;
          expr_size++;
        }
    /*
        Serial.print("updated expr[");
        Serial.print(expr_size);
        Serial.print("]: \"");
        Serial.print(expression);
        Serial.println("\"");
    */
    /*
        sprintf(buffer1, "%.*s\n", LCDCOLS, expression);
        Serial.print("buffer1: \"");
        Serial.print(buffer1);
        Serial.println("\"");
    */

        window_start = 0;
        if (expr_size-LCDCOLS > 0){
          window_start = expr_size-LCDCOLS;
        }
        window_end = window_start+LCDCOLS;
        if (window_end > (EXPRSIZE-1)){
          window_end = EXPRSIZE-1;
        }

        clear_display();
        alpha_board.write_string(expression[window_start],0,0);
        //for (int i=window_start; i<window_end; i++){
          //lcd.write(expression[i]);
        //}
      }

      if (eval) {
        result = eval_expression();
        if (result != NULL) {
          /*
          Serial.print("result: ");
          Serial.println(result);
          */
          clear_display();
          alpha_board.write_string(expression[window_start],0,0);
          sprintf(buffer, "%f", result);
          alpha_board.write_string(buffer,1,0);
          clear_stack();
        }
        eval = 0;
      }
    }
  }
}

