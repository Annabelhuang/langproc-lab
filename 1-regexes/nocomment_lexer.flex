%option noyywrap

%{

#include "nocomment.hpp"

// The following line avoids an annoying warning in Flex
// See: https://stackoverflow.com/questions/46213840/
extern "C" int fileno(FILE *stream);

int comment_count = 0;  // Comment count variable
int attribute_count = 0;  // Attribute count variable

%}
%x attribute

%%
"\\"[^ ]* {
  printf("%s", yytext);
  }

"//".*\n {
  return Comment;
}


"\(\*" {
  BEGIN(attribute);
}

"\\"[^ ]* {
  printf("%s", yytext);
}

<attribute>"//".*\n {
}

<attribute>([^*]|\*[^)]|[\n]) {
  // Consume and ignore lines within the attribute state
}

<attribute>"\*\)" {
  BEGIN(INITIAL);
  return Attribute;
}


. {
  yylval.character = yytext[0];
  return Other;
}

<<EOF>> {
    return Eof;
}

%%

/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s)
{
  fprintf (stderr, "Flex Error: %s\n", s);
  exit(1);
}
