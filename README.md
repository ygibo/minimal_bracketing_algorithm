# minimal_bracketing_algorithm
An algorithm for finding the minimum bracket of an arithmetic expression.

 アルゴリズムの概要
 入力　(1*2)+3<<2+(4&6) のような算術子とビット演算子をもつ式
 出力  1*2+3<<2+4&6 のような無駄な括弧を省いた式

 例えば、1+2,3+4 を掛けて (1+2)*(3+4) にする場合、1+2,3+4 をそのまま'*'で結合した場合、
 1+2*3+4 となり意味が変わってしまう。これは、'*'の両隣にある'+'が'*'より優先度が低いためである。
 そのため、1+2 の '+' に着目したとき、'+'の優先度が'*'より低い場合に括弧が必要になるとわかる。
 以上のように、隣同士にある演算子の優先度を比較することで括弧が必要かが分かる。
 
 そこで隣同士にある演算子をより簡単に比較するため、与えられた式を構文解析し、解析木上で
 演算子を比較することで括弧が必要かを判断し、無駄な括弧がない式を出力する。
 構文解析木上では、各ノードの持つ演算子と親ノードが持つ演算子の優先度を
 比較することで、元の式上で隣り合った演算子の優先度を比較することが簡単にできる。


 コードの概要
 show_waste_free_expression(Operand prev_op, Node* node)関数が構文解析木上の演算子を
 再帰的に比較し、無駄な括弧の無い式を出力する関数である。
 この関数では、第一引数に現在のノードの親ノードの演算子、第二引数に
 解析木を渡す。そのため、コードの初めのほうのparse_が頭につく関数により、構文解析を行い
 解析木を作成する。
 また、入力としては、一つの式が一行で与えられているものとする。そうでない場合は、
 次の入力文字を得る get_next 関数を少し変えることで対応できる。
 
 プログラムへの入力と出力
 プログラムへの入力は、標準入力から行う。標準入力からの１行分が一つの式として入力される。
 出力も標準出力に出力される。字句解析を導入することで複数行からなる式も解析できるよう
 プログラムを変更できる。

