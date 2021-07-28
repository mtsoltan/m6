#include <TokenTypeChecker.h>

TokenTypeChecker::TokenTypeChecker (int log_handler(const char*, ...)) {
    this->log_handler = log_handler;
}

/**
 * Once we know that we've encountered a symbol, we can process it using this method.
 * This method will change the position of this->tokenizer_iterator to after the identifier,
 * allowing us to continue processing.
 * @return
 */
bool TokenTypeChecker::process_symbol () {
    // Reaching this point means that we have a punctuation symbol.
    // First, we need to recognize possible operators based on the first punctuation mark,
    // then compare the second, third, etc. till we settle on a specific operator.
    // TODO: https://github.com/mtsoltan/m6/issues/7

    if (*this->tokenizer_iterator == '=')  {
        // Assignment, arrow, equality, strict equality.
    }
    if (*this->tokenizer_iterator == '+')  {
        // Add assignment, addition, unary plus, increment.
    }
    if (*this->tokenizer_iterator == '-')  {
        // Subtract assignment, subtraction, unary minus, decrement.
    }
    if (*this->tokenizer_iterator == '*')  {
        // Multiply assignment, multiplication, exponentiation, exponentiation assignment.
        // Block comment end cannot be encountered first and is thus ignored.
    }
    if (*this->tokenizer_iterator == '/')  {
        // Division assignment, division, comment, block comment,
    }
    if (*this->tokenizer_iterator == '%')  {
        // Remainder assignment, remainder.
    }
    if (*this->tokenizer_iterator == '&')  {
        // Logical and assignment, bitwise and assignment, logical and, bitwise and.
    }
    if (*this->tokenizer_iterator == '|')  {
        // Logical or assignment, bitwise or assignment, logical or, bitwise or.
    }
    if (*this->tokenizer_iterator == '~')  {
        // Bitwise not assignment, bitwise not.
    }
    if (*this->tokenizer_iterator == '!')  {
        // Logical not assignment, logical not, not equal, strict not equal.
    }
    if (*this->tokenizer_iterator == '^')  {
        // Logical xor assignment, logical xor.
    }
    if (*this->tokenizer_iterator == '<')  {
        // Less than, less than or equal to, left shift, integer left shift.
    }
    if (*this->tokenizer_iterator == '>')  {
        // Greater than, greater than or equal to, right shift, integer right shift.
    }
    if (*this->tokenizer_iterator == '.')  {
        // Chaining, ellipsis.
    }
    if (*this->tokenizer_iterator == '?')  {
        // Optional chaining, coalescing, trinary conditional.
    }
    if (*this->tokenizer_iterator == ',')  {
        // Joining
    }
    if (*this->tokenizer_iterator == ':')  {
        // Trinary fallback, case end.
    }
    if (*this->tokenizer_iterator == '"')  {
        // Double string start.
        // Double string end cannot be encountered first and is thus ignored.
    }
    if (*this->tokenizer_iterator == '\'')  {
        // Single string start.
        // Single string end cannot be encountered first and is thus ignored.
    }
    if (*this->tokenizer_iterator == '`')  {
        // Template literal start.
        // Template literal end cannot be encountered first and is thus ignored.
    }
    if (*this->tokenizer_iterator == '(')  {
        // Starting parentheses.
    }
    if (*this->tokenizer_iterator == ')')  {
        // Ending parentheses.
    }
    if (*this->tokenizer_iterator == '[')  {
        // Access operator, append operator, array literal start.
    }
    if (*this->tokenizer_iterator == ']')  {
        // Ending bracket.
    }
    if (*this->tokenizer_iterator == '{')  {
        // Starting curly brace, object literal start.
    }
    if (*this->tokenizer_iterator == '}')  {
        // Ending curly brace.
    }

    ++this->tokenizer_iterator;
    return true;
}

int64_t TokenTypeChecker::get_char_offset () {
    if (this->tokenizer_iterator == this->tokenizer_string.end() || *this->tokenizer_iterator == '\0') {
        return NOT_FOUND;  // This whole function needs to be signed, because NOT_FOUND is negative.
    }

    return this->tokenizer_iterator - this->tokenizer_string.begin();
}

bool TokenTypeChecker::next_token_is_number () {
    std::string::iterator temp = this->tokenizer_iterator;

    // If it starts with a negative sign, then we ignore it, since it might very well be a number.
    if (*temp == '-') {
        ++temp;
    }

    // Falling through from the negative sign (order is important),
    // if it starts with a dot, then we ignore it, since it might very well be a number.
    if (*temp == '.') {
        ++temp;
    }

    // Falling back through from the optional negative sign and dot,
    // anything that starts with a digit is automatically a number.
    return Token::is_digit(*temp);
}

opcode_t TokenTypeChecker::next_token_is_keyword () {
    std::string::iterator end = this->tokenizer_string.end() - this->tokenizer_iterator > OP_KEYWORD_SIZE ?
                                this->tokenizer_iterator + OP_KEYWORD_SIZE :
                                this->tokenizer_string.end();
    return Token::cstr_to_keyword(std::string(this->tokenizer_iterator, end).c_str());
}
