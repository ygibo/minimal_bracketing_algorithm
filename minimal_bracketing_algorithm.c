/* �A���S���Y���̊T�v
 * ���́@(1*2)+3<<2+(4&6) �̂悤�ȎZ�p�q�ƃr�b�g���Z�q������
 * �o��  1*2+3<<2+4&6 �̂悤�Ȗ��ʂȊ��ʂ��Ȃ�����
 *
 * �Ⴆ�΁A1+2,3+4 ���|���� (1+2)*(3+4) �ɂ���ꍇ�A1+2,3+4 �����̂܂�'*'�Ō��������ꍇ�A
 * 1+2*3+4 �ƂȂ�Ӗ����ς���Ă��܂��B����́A'*'�̗��ׂɂ���'+'��'*'���D��x���Ⴂ���߂ł���B
 * ���̂��߁A1+2 �� '+' �ɒ��ڂ����Ƃ��A'+'�̗D��x��'*'���Ⴂ�ꍇ�Ɋ��ʂ��K�v�ɂȂ�Ƃ킩��B
 * �ȏ�̂悤�ɁA�ד��m�ɂ��鉉�Z�q�̗D��x���r���邱�ƂŊ��ʂ��K�v����������B
 * 
 * �����ŗד��m�ɂ��鉉�Z�q�����ȒP�ɔ�r���邽�߁A�^����ꂽ�����\����͂��A��͖؏��
 * ���Z�q���r���邱�ƂŊ��ʂ��K�v���𔻒f���A���ʂȊ��ʂ��Ȃ������o�͂���B
 * �\����͖؏�ł́A�e�m�[�h�̎����Z�q�Ɛe�m�[�h�������Z�q�̗D��x��
 * ��r���邱�ƂŁA���̎���ŗׂ荇�������Z�q�̗D��x���r���邱�Ƃ��ȒP�ɂł���B
 *
 *
 * �R�[�h�̊T�v
 * show_waste_free_expression(Operand prev_op, Node* node)�֐����\����͖؏�̉��Z�q��
 * �ċA�I�ɔ�r���A���ʂȊ��ʂ̖��������o�͂���֐��ł���B
 * ���̊֐��ł́A�������Ɍ��݂̃m�[�h�̐e�m�[�h�̉��Z�q�A��������
 * ��͖؂�n���B���̂��߁A�R�[�h�̏��߂̂ق���parse_�����ɂ��֐��ɂ��A�\����͂��s��
 * ��͖؂��쐬����B
 * �܂��A���͂Ƃ��ẮA��̎�����s�ŗ^�����Ă�����̂Ƃ���B�����łȂ��ꍇ�́A
 * ���̓��͕����𓾂� get_next �֐��������ς��邱�ƂőΉ��ł���B
 * 
 * �v���O�����ւ̓��͂Əo��
 * �v���O�����ւ̓��͂́A�W�����͂���s���B�W�����͂���̂P�s������̎��Ƃ��ē��͂����B
 * �o�͂��W���o�͂ɏo�͂����B�����͂𓱓����邱�Ƃŕ����s����Ȃ鎮����͂ł���悤
 * �v���O������ύX�ł���B
 */
#define MAX_STRING_SIZE 1024
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

typedef enum{
  Mul, Div, Mod, Plus, Minus, Shift_Left, Shift_Right, And, Ex_Or, Or, Number
} Operand;

// enum �Œ�`�������ɁA���Z�q�̗D��x��ݒ�i�Ⴂ�قǗD��x�������j
int operand_priority[] = { 1,1,1,2,2,3,3,4,5,6,7 }; 
char operand_table[] = { '*', '/', '%', '+', '-', '<', '>', '&', '^', '|', 'N' };

// ���Z�q�̗D��x�̔�r�֐�
int compare_operand(Operand left,Operand right){
  return operand_priority[left] < operand_priority[right];
}

// �\����͖ؗp�̃m�[�h�A���l�̏ꍇ��num�ɂ��̐��l�Aleft,right��NULL�ɐݒ肷��
struct Node{
  Operand op;
  int num;
  struct Node* left;
  struct Node* right;
};

typedef struct Node Node;

// �m�[�h�R���X�g���N�^
Node* get_new_node(Operand op, int num, Node* left, Node* right){
  Node* new_node = NULL;
  new_node = (Node*)malloc(sizeof(Node));
  new_node->left = left;
  new_node->right = right;
  new_node->op = op;
  new_node->num = num;
  return new_node;
}

// �ċA���~�ō\����͂�����
/* �\���́AExtend Buckus-Naur Form ���ۂ����̂ŁA
expression        := ex_term { '|' ex_term }
ex_term           := and_term { '^' and_term }
and_term          := shift_term { '&' shift_term }
shift_term        := plus_minus_term { ('<<' | '>>') plus_minus_term }
plus_minus_term   := div_mul_mod_term { ('+'|'-') div_mul_mod_term }
div_mul_mode_term := factor { ('*'|'/'|'%') factor }
factor            := { Number }
�ƂȂ�B�������̊֐������ɂ��ꂼ��̍\����͂ɑΉ�����
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
//���̕�����ǂݍ���
char get_next(char* parse_obj){
  while(parse_obj[exp_index]==' ')
    exp_index++;
  return parse_obj[exp_index];
}

//�p�[�T�[�֐�
// '|'�ł���킳��� or �\�������
Node* parse_expression(char* parse_obj){
  Node* node = parse_ex_term(parse_obj);
  while( get_next(parse_obj) == '|' ){
    ++exp_index;
    node = get_new_node( Or, 0, node, parse_ex_term(parse_obj) );
  }
  return node;
}

// xor ���
Node* parse_ex_term(char* parse_obj){
  Node* node = parse_and_term(parse_obj);
  while( get_next(parse_obj) == '^' ){
    ++exp_index;
    node = get_new_node( Ex_Or, 0, node, parse_and_term(parse_obj) );
  }
  return node;
}

// and �����
Node* parse_and_term(char* parse_obj){
  Node* node = parse_shift_term(parse_obj);
  while( get_next(parse_obj) == '&' ){
    ++exp_index;
    node = get_new_node( And, 0, node, parse_shift_term(parse_obj) );
  }
  return node;
}

// �r�b�g�V�t�g�����
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

// +,- �����
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

// ����Z�A�|���Z�A���W�������Z�����
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

// ���l�����
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

//���Z�q�̕\���֐�
void show_operand(Operand op){
  if(op == Shift_Left)
    printf("<<");
  else if(op == Shift_Right)
    printf(">>");
  else
    printf("%c",operand_table[op]);
}

//�S���̊��ʂ�t���ĕ\���A�f�o�b�O�Ɏg�p
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

//���ʂȊ��ʂ���菜���ĕ\������֐�,�������͐e�̃m�[�h�̉��Z�q��n��
void show_waste_free_expression(Operand prev_op, Node* node){
  if(node == NULL)
    return ;
  if(node->op == Number){
    printf("%d", node->num);
    return ;
  }else{ 
    // ���݂̃m�[�h�ƈ��̃m�[�h�̉��Z�q���r���A
    // ���݂̃m�[�h�̗D��x�������Ȃ犇�ʂ�����Ȃ�
    if(compare_operand( prev_op, node->op ))
      printf("(");
    show_waste_free_expression(node->op,node->left);
    show_operand(node->op);
    show_waste_free_expression(node->op,node->right);
    if(compare_operand( prev_op, node->op ))
      printf(")");
  }
}

//�\����͖؂̃����������
void free_parse_tree(Node* node){
  if(node == NULL)
    return ;
  free(node->left);
  free(node->right);
  free(node);
}

int main(){
  char parse_obj[MAX_STRING_SIZE];
  // �W�����͂���1�s���ǂݍ��� 
  while(fgets(parse_obj,1024,stdin) != NULL){

    init_parser();
    //parse����
    Node* node = parse_expression( parse_obj );
    //���ʂȊ��ʂ̖�������\������
    show_waste_free_expression( Number, node);
    printf("\n");
    free_parse_tree( node );
  }
  return 0;
}
