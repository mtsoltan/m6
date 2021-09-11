#ifndef M6_KEYWORDBALANCER_H
#define M6_KEYWORDBALANCER_H

#include <Tokenizer.h>

class KeywordBalancer: public Tokenizer {
public:
    explicit KeywordBalancer (int log_handler (const char*, ...));

    /*
     * Start with keywords -> check the no-para ones, then the optional para-ones
     * Then the strict para ones, then the strict para strict format ones.
     * Check the ones with a scope / single-statement after see if it exists.
     *
     * End them all with EOS (loops, function definitons, conditions, whatnot)
     *
     * Throw error if needed.
     *
     * Take care of arrow function definitions. todo
     * some paras / braces will be left:
     *
     * ========
     *
     * paras are either fn calls, or grouping.
     *
     * non-arrow definitions are already taken care of using keywords, and arrow function definitions
     * are taken care of right after, so no remaining function definitions are possible.
     *
     * Is preceeded by callable (same as regex semantics, closing ]}) are callable)
     * yes -> call
     * no -> grouping
     *
     * ========
     *
     * braces are either object literals or scoping. scoping can only follow EOS.
     *
     * how to know if braces are a scope or an object literal?
     * According to [#prod-ExpressionStatement], a statement cannot start with a LEFT CURLY BRACKET.
     */

    bool classify_parentheses();

    bool classify_braces();

    bool classify_brackets();

    bool process_scope();

    bool process_grouping();

protected:
    bool process_next_token ();
};


#endif
