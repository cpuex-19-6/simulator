# simulator ver2

ver1から内部仕様を大幅に変更しました。

## ISA

RV32IF + alpha

## ビルド方法

`make`

## 実行方法

シミュレートしたい命令ファイルを`instr_file`としたときに
ファイルがバイナリの場合`./sim instr_file`

ファイルがテキストの場合`./sim -t instr_file`

命令ファイルは必須です。ただし、入出力ファイルやセットアップフィル、タグファイルを設定したい場合は下に示すようなオプションで設定可能です。
  
使用ファイルのオプションをつけて実行した後にコマンドを打ってシミュレーションを行えます。  

シミュレーションの実行が終了すると、すべてのレジスタの内容と、解析結果を出力します。

## 注意!!!!!!
入出力命令を使用するには、入出力用のファイルを設定しないとダメです。
  
## file option

`-t txt-instruction-file` （命令ファイルはデフォルト(オプション無し）ではバイナリ）

`-db binary-data-file`

`-dt txt-data-file`

`-ob binary-output-file`

`-ot txt-output-file`

`-s setup-file`

`-l tag-file`

## tag file

オプションでタグファイルを設定することで、実行中に何度そのタグを訪れたかを解析し、その結果を表示します。
タグファイルは以下のように、タグ名の次に対応する部分の最初の命令のアドレスを書いたものです。

```
entry
0
function-1
4
function-2
88
```

## setup file

上述しましたが、`-s`オプションでセットアップファイルを設定できます。セットアップファイルには以下に示すコマンドを書くことができ、シミュレーター起動時にそれらが順に実行されます。
セットアップファイルは例えば以下のような書式です。

```
x 0
x 1

miw 0

s

b 116

r

sn 10
```

## command

`b [d]` [d]番目の命令にブレークポイントを設定する

`r` 次の命令から設定したブレークポイントまで一気に実行（ブレークポイントが無ければ最後まで）

`re` 次の命令から最後（フェッチした命令語が32bit 0 となるまで）まで一気に実行

`s` 次の命令を逆アセンブルした結果を命令の二進表現とともに表示し、その命令だけを実行

`sn [d]` ステップ実行を一気に[d]回行う

`x [d]` インデックスが[d]のレジスタの中身を10進数でステップ実行の際に常に表示

`xx [d]` インデックスが[d]のレジスタの中身を16進数でステップ実行の際に常に表示（ただし、一度16進表示にするとそのレジスタはずっと16進数表示）

`f [d]` インデックスが[d]の浮動小数点数レジスタの中身をステップ実行の際に常に表示

`m [type][size] [address]` メモリの[address]番地から[size]分だけを[type]型でステップ実行の際に表示、また、全命令を実行した後にも表示される`i`でint、`u`でunsigned int、`f`でfloat。`b`でbyte、`h`でhalf word、`w`でword。

`q` その時点で終了

## 注意書き

浮動小数点数の計算は `FMUL`, `FDIV`, `FSQRT` のみ実機FPUの実装に合わせ、それ以外はCライブラリのものを用いる。

rounding modeは今のところは000すなわちRNEのみを許すようにしてある。
