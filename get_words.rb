
word = /".*"|char|short|int|unsigned|long|float|double|struct|union|void|enum|signed|const|volatile|typedef|auto|register|static|extern|break|case|continue|default|do|else|for|goto|if|return|switch|while|sizeof|\d+|\d+\.\d*|\d*\.\d+|[_a-zA-Z][a-zA-Z0-9_]*|\+|-|\*|%|==|=|!=|,|\(|\)|\[|\]|\{|\}|<|>|;/

num = /\d+/
real = /\d+\.\d*|\d*\.\d+/
key_word = /char|short|int|unsigned|long|float|double|struct|union|void|enum|signed|const|volatile|typedef|auto|register|static|extern|break|case|continue|default|do|else|for|goto|if|return|switch|while|sizeof/
id = /[_a-zA-Z][a-zA-Z0-9_]*/
str = /\".\"/
op = /\+|-|\*|%|==|=|!=/
comma = /,/
simicon = /;/
parent = /\(|\)|\[|\]|\{|\}|<|>/

num_set = []
real_set = []
key_word_set = []
id_set = []
str_set = []
op_set = []
comma_set = []
parent_set = []
simicon_set = []

word_set = []
File.open("test.c").each do |line|
  line.scan(word).each do |w|
    #puts w
    case w
    when num        then num_set        << w; word_set << [w,"number"]    
    when real       then real_set       << w; word_set << [w,"real"]      
    when key_word   then key_word_set   << w; word_set << [w,"key_word"]  
    when id         then id_set         << w; word_set << [w,"id"]        
    when str        then str_set        << w; word_set << [w,"string"]    
    when op         then op_set         << w; word_set << [w,"operator"]  
    when comma      then comma_set      << w; word_set << [w,"comma"]     
    when parent     then parent_set     << w; word_set << [w,"parent"]    
    when simicon    then simicon_set    << w; word_set << [w,"simicon"]   
    end
  end
end

out = File.new("words","w")
word_set.each {|e| out.printf "%10s :  %s\n", e[1], e[0]}
