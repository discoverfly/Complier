
words = []
word = /".*"|char|short|int|unsigned|long|float|double|struct|union|void|enum|signed|const|volatile|typedef|auto|register|static|extern|break|case|continue|default|do|else|for|goto|if|return|switch|while|sizeof|\d+|\d+\.\d*|\d*\.\d+|[_a-zA-Z][a-zA-Z0-9_]*|\+|-|\*|%|==|=|!=|,|\(|\)|\[|\]|\{|\}|<|>/

num = /\d+/
real = /\d+\.\d*|\d*\.\d+/
key_word = /char|short|int|unsigned|long|float|double|struct|union|void|enum|signed|const|volatile|typedef|auto|register|static|extern|break|case|continue|default|do|else|for|goto|if|return|switch|while|sizeof/
id = /[_a-zA-Z][a-zA-Z0-9_]*/
str = /\".\"/
op = /\+|-|\*|%|==|=|!=/
comma = /,/
parent = /\(|\)|\[|\]|\{|\}|<|>/
num_set = []
real_set = []
key_word_set = []
id_set = []
str_set = []
op_set = []
comma_set = []
parent_set = []

File.open("test.c").each do |line|
  line.scan(word).each{|w| puts w}
end

puts num_set
puts key_word_set
puts id_set
