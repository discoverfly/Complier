
word = /".*"|char|short|int|unsigned|long|float|double|struct|union|void|enum|signed|const|volatile|typedef|auto|register|static|extern|break|case|continue|default|do|else|for|goto|if|return|switch|while|sizeof|\d+|\d+\.\d*|\d*\.\d+|[_a-zA-Z][a-zA-Z0-9_]*|\+\+|\+|-|\*|%|==|=|!=|,|\(|\)|\[|\]|\{|\}|<|>|;/
num = /\d+/
real = /\d+\.\d*|\d*\.\d+/
key_word = /char|short|int|unsigned|long|float|double|struct|union|void|enum|signed|const|volatile|typedef|auto|register|static|extern|break|case|continue|default|do|else|for|goto|if|return|switch|while|sizeof/
id = /[_a-zA-Z][a-zA-Z0-9_]*/
str = /".*"/
op = /\+\+|\+|-|\*|%|==|=|!=/
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
    if (line =~ /#include.*<.*>/) == 0 then next end
    line.scan(word).each do |w|
        #puts w , (str =~ w)
        if w == "main" then
            word_set << [w,w]
        elsif (num =~ w) == 0 then 
            num_set        << w; word_set << [w,"DIGIT"]   
        elsif (real =~ w) == 0 then
            real_set       << w; word_set << [w,"REAL"]      
        elsif (key_word =~ w) == 0   then
            key_word_set   << w; word_set << [w,w]  
        elsif (id =~ w) == 0 then
            id_set         << w; word_set << [w,"ID"]        
        elsif (str =~ w) == 0 then
            str_set << w; word_set << [w,"STRING"]    
        elsif (op =~ w) == 0 then
            op_set         << w; word_set << [w,w]  
        elsif (comma =~ w) == 0 then
            comma_set      << w; word_set << [w,w]     
        elsif (parent =~ w) == 0 then
            parent_set     << w; word_set << [w,w]    
        elsif (simicon =~ w) == 0 then 
            simicon_set    << w; word_set << [w,"#"]   
        end
    end
end

out = File.new("words","w")
word_set.each {|e| out.printf "%10s :  %s\n", e[1], e[0]; print e[1], " "}
out.puts " $ : $"
