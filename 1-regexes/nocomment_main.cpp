#include "nocomment.hpp"

#include <iostream>
#include <cassert>

// This is the yylval variable that the nocomment.hpp header file refers to.
TokenValue yylval;
extern int comment_count;
extern int attribute_count;

int main () { 

    while (1) {

        // Lex the next character in the input stream.
        TokenType type = (TokenType) yylex();
    
        if (type == Eof) {
      
            break;
        } 
        else if (type == Comment || type == Attribute) {
            // If it's a comment or attribute, we do not output anything,
            // and just increment the appropriate counter.
            if (type == Comment) {
                comment_count++;
            } else if (type == Attribute) {
                attribute_count++;
            }
        }

        else if (type == Other) {
            std::cout << yylval.character;
        } 
        else {
            assert(0); // Error out!
            return 1;
        }
    }

  std::cout << "Number of comments and attributes removed: "
            << comment_count + attribute_count << ".\n";
    
  return 0;
}
