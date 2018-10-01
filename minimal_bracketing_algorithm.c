/* アルゴリズムの概要
 * 入力　(1*2)+3<<2+(4&6) のような算術子とビット演算子をもつ式
 * 出力  1*2+3<<2+4&6 のような無駄な括弧を省いた式
 *
 * 例えば、1+2,3+4 を掛けて (1+2)*(3+4) にする場合、1+2,3+4 をそのまま'*'で結合した場合、
 * 1+2*3+4 となり意味が変わってしまう。これは、'*'の両隣にある'+'が'*'より優先度が低いためである。
 * そのため、1+2 の '+' に着目したとき、'+'の優先度が'*'より低い場合に括弧が必要になるとわかる。
 * 以上のように、隣同士にある演算子の優先度を比較することで括弧が必要かが分かる。
 * 
 * そこで隣同士にある演算子をより簡単に比較するため、与えられた式を構文解析し、解析木上で
 * 演算子を比較することで括弧が必要かを判断し、無駄な括弧がない式を出力する。
 * 構文解析木上では、各ノードの持つ演算子と親ノードが持つ演算子の優先度を
 * 比較することで、元の式上で隣り合った演算子の優先度を比較することが簡単にできる。
 *
 *
 * コードの概要
 * show_waste_free_expression(Operand prev_op, Node* node)関数が構文解析木上の演算子を
 * 再帰的に比較し、無駄な括弧の無い式を出力する関数である。
 * この関数では、第一引数に現在のノードの親ノードの演算子、第二引数に
 * 解析木を渡す。そのため、コードの初めのほうのparse_が頭につく関数により、構文解析を行い
 * 解析木を作成する。
 * また、入力としては、一つの式が一行で与えられているものとする。そうでない場合は、
 * 次の入力文字を得る get_next 関数を少し変えることで対応できる。
 * 
 * プログラムへの入力と出力
 * プログラムへの入力は、標準入力から行う。標準入力からの１行分が一つの式として入力される。
 * 出力も標準出力に出力される。字句解析を導入することで複数行からなる式も解析できるよう
 * プログラムを変更できる。
 */
#define MAX_STRING_SIZE 1024
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

typedef enum{
  Mul, Div, Mod, Plus, Minus, Shift_Left, Shift_Right, And, Ex_Or, Or, Number
} Operand;

// enum で定義した順に、演算子の優先度を設定（低いほど優先度が高い）
int operand_priority[] = { 1,1,1,2,2,3,3,4,5,6,7 }; 
char operand_table[] = { '*', '/', '%', '+', '-', '<', '>', '&', '^', '|', 'N' };

// 演算子の優先度の比較関数
int compare_operand(Operand left,Operand right){
  return operand_priority[left] < operand_priority[right];
}

// 構文解析木用のノード、数値の場合はnumにその数値、left,rightはNULLに設定する
struct Node{
  Operand op;
  int num;
  struct Node* left;
  struct Node* right;
};

typedef struct Node Node;

// ノードコンストラクタ
Node* get_new_node(Operand op, int num, Node* left, Node* right){
  Node* new_node = NULL;
  new_node = (Node*)malloc(sizeof(Node));
  new_node->left = left;
  new_node->right = right;
  new_node->op = op;
  new_node->num = num;
  return new_node;
}

// 再帰下降で構文解析をする
/* 構文は、Extend Buckus-Naur Form っぽいもので、
expression        := ex_term { '|' ex_term }
ex_term           := and_term { '^' and_term }
and_term          := shift_term { '&' shift_term }
shift_term        := plus_minus_term { ('<<' | '>>') plus_minus_term }
plus_minus_term   := div_mul_mod_term { ('+'|'-') div_mul_mod_term }
div_mul_mode_term := factor { ('*'|'/'|'%') factor }
factor            := { Number }
となる。すぐ下の関数が順にそれぞれの構文解析に対応する
*/
Node* parse_expression(char* parse_obj);
Node* parse_ex_term(char* parse_obj);
Node* parse_and_term(char* parse_obj);
Node* parse_shift_term(char* parse_obj);
Node* parse_plus_minus_term(char* parse_obj);
Node* parse_div_mul_mod_term(char* parse_obj);
Node* factor(char* parse_obj);


int exp_index;
void init_parser(){
  exp_index = 0;
}
//次の文字を読み込む
char get_next(char* parse_obj){
  while(parse_obj[exp_index]==' ')
    exp_index++;
  return parse_obj[exp_index];
}

//パーサー関数
// '|'であらわされる or 構文を解析
Node* parse_expression(char* parse_obj){
  Node* node = parse_ex_term(parse_obj);
  while( get_next(parse_obj) == '|' ){
    ++exp_index;
    node = get_new_node( Or, 0, node, parse_ex_term(parse_obj) );
  }
  return node;
}

// xor 解析
Node* parse_ex_term(char* parse_obj){
  Node* node = parse_and_term(parse_obj);
  while( get_next(parse_obj) == '^' ){
    ++exp_index;
    node = get_new_node( Ex_Or, 0, node, parse_and_term(parse_obj) );
  }
  return node;
}

// and を解析
Node* parse_and_term(char* parse_obj){
  Node* node = parse_shift_term(parse_obj);
  while( get_next(parse_obj) == '&' ){
    ++exp_index;
    node = get_new_node( And, 0, node, parse_shift_term(parse_obj) );
  }
  return node;
}

// ビットシフトを解析
Node* parse_shift_term(char* parse_obj){
  Node* node = parse_plus_minus_term(parse_obj);
  while( (get_next(parse_obj) == '<' & parse_obj[exp_index+1] == '<') ||
	 (get_next(parse_obj) == '>' & parse_obj[exp_index+1] == '>') ){
    if(get_next(parse_obj) == '<' & parse_obj[exp_index+1] == '<'){
      exp_index++;
      exp_index++;
      node = get_new_node( Shift_Left, 0, node, parse_plus_minus_term(parse_obj));
    }else if(get_next(parse_obj) == '>' & parse_obj[exp_index+1] == '>'){
      exp_index++;
      exp_index++;
      node = get_new_node( Shift_Right, 0, node, parse_plus_minus_term(parse_obj));
    }
  }
  return node;
}

// +,- を解析
Node* parse_plus_minus_term(char* parse_obj){
  Node* node = parse_div_mul_mod_term(parse_obj);
  while( get_next(parse_obj) == '+' || get_next(parse_obj) == '-' ){
    if( get_next(parse_obj) == '+' ){
      ++exp_index;
      node = get_new_node( Plus, 0, node, parse_div_mul_mod_term(parse_obj) );
    }else if( get_next(parse_obj) == '-'){
      ++exp_index;
      node = get_new_node( Minus, 0, node, parse_div_mul_mod_term(parse_obj) );
    }
  }
  return node;
}

// 割り算、掛け算、モジュロ演算を解析
Node* parse_div_mul_mod_term(char* parse_obj){
  Node* node = factor(parse_obj);
  while( get_next(parse_obj) == '*' || get_next(parse_obj) == '/' ||
	 get_next(parse_obj) == '%' ){
    if( get_next(parse_obj) == '*' ){
      exp_index++;
      node = get_new_node( Mul, 0, node, factor(parse_obj));
    }else if( get_next(parse_obj) == '/'){
      exp_index++;
      node = get_new_node( Div, 0, node, factor(parse_obj));
    }else if( get_next(parse_obj) == '%'){
      exp_index++;
      node = get_new_node( Mod, 0, node, factor(parse_obj));
    }
  }
  return node;
}

// 数値を解析
Node* factor(char* parse_obj){
  Node* node = NULL;
  if( get_next(parse_obj) == '(' ){
    ++exp_index; node = parse_expression(parse_obj); ++exp_index;
  }else{
    int value = 0;
    while( isdigit(get_next(parse_obj)) ){
      value = value * 10 + (int)(get_next(parse_obj)-'0');
      exp_index++;
    }
    node = get_new_node( Number, value, NULL, NULL );
  }
  return node;
}

//演算子の表示関数
void show_operand(Operand op){
  if(op == Shift_Left)
    printf("<<");
  else if(op == Shift_Right)
    printf(">>");
  else
    printf("%c",operand_table[op]);
}

//全部の括弧を付けて表示、デバッグに使用
void dump_parse_tree(Node* node){
  if(node == NULL)
    return ;
  if(node->op == Number){
    printf("%d", node->num);
    return ;
  }else{
    printf("(");
    dump_parse_tree(node->left);
    show_operand(node->op);
    dump_parse_tree(node->right);
    printf(")");
  }
}

//無駄な括弧を取り除いて表示する関数,第一引数は親のノードの演算子を渡す
void show_waste_free_expression(Operand prev_op, Node* node){
  if(node == NULL)
    return ;
  if(node->op == Number){
    printf("%d", node->num);
    return ;
  }else{ 
    // 現在のノードと一つ上のノードの演算子を比較し、
    // 現在のノードの優先度が高いなら括弧がいらない
    if(compare_operand( prev_op, node->op ))
      printf("(");
    show_waste_free_expression(node->op,node->left);
    show_operand(node->op);
    show_waste_free_expression(node->op,node->right);
    if(compare_operand( prev_op, node->op ))
      printf(")");
  }
}

//構文解析木のメモリを解放
void free_parse_tree(Node* node){
  if(node == NULL)
    return ;
  free(node->left);
  free(node->right);
  free(node);
}

int main(){
  char parse_obj[MAX_STRING_SIZE];
  // 標準入力から1行ずつ読み込む 
  while(fgets(parse_obj,1024,stdin) != NULL){

    init_parser();
    //parseする
    Node* node = parse_expression( parse_obj );
    //無駄な括弧の無い式を表示する
    show_waste_free_expression( Number, node);
    printf("\n");
    free_parse_tree( node );
  }
  return 0;
}
