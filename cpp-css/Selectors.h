#pragma once

////////////////////////////////////////////////////////////
//
// MIT License
//
// Copyright(c) 2017 Kurt Slagle - kurt_slagle@yahoo.com
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// The origin of this software must not be misrepresented; you must not claim
// that you wrote the original software.If you use this software in a product,
// an acknowledgment of the software used is required.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Internal Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////
#include <iostream>
#include <istream>
#include <string>
#include <vector>
#include <sstream>

////////////////////////////////////////////////////////////
//  What is this?
//   - A basic set of CSS parsing classes
//
//  Why not use X parser?
//   - I do not want to rely on external libs 
//     unless I absolutely have to
//   - None of the well-known css-parsers are
//     easy to use
//
//  Is it tested?
//   - Yes. Tests.cpp contains all tests
//   - You can run the tests just by compiling
//     Selectors.h/cpp and Tests.cpp
//   - Code will not be published that does not pass
//     all tests
//   - I have major gripes with TDD.  
//     I do not do data-driven tests.
//     My tests are behavior driven. 
//     If a test is not shown, it is likely the context
//       under which it occurs was not thought of
//       or has not been experienced yet.
//     But behavior-driven tests play nicely with agile
//       development and thus can adapt easily to new
//       behaviors
//     GIVEN ( some initial context )
//     WHEN  ( certain criteria are met )
//     THEN  ( certain behavior is expected )
//
//  It's broken!
//   - Please open an issue and demonstrate the issue
//   - Provide the text you tried to parse and where
//     the issue occurs
//
////////////////////////////////////////////////////////////

namespace css
{

#define REPORT_PARSE_FAILURE_AND_RETURN(ERR_MSG, RET_VAL) \
{ \
std::cout << "Parse failure: " << ERR_MSG << "\n"; \
return RET_VAL; \
}

#define REPORT_IO_SOURCE_INVALID_AND_RETURN(RET_VAL) \
{ \
std::cout << "Parse failure: Input stream is in an invalid state\n"; \
return RET_VAL; \
}

  __forceinline void IgnoreWhitespace(std::istream &Input) { while (Input && isspace(Input.peek())) Input.ignore(); }
  __forceinline std::string IgnoreAndAccumulateWhitespace(std::istream &Input)
  {
    std::string tmp{ "" };
    while (Input && isspace(Input.peek())) {
      tmp += Input.get();
    }

    return tmp;
  }

  __forceinline bool IsOneOf(char c, const std::string &str) { return str.find(c) != std::string::npos; }

  __forceinline void UndoExtraction(std::istream &Input, const std::string &ToPutBack)
  {
    auto rbeg = ToPutBack.crbegin();
    auto rend = ToPutBack.crend();

    for (rbeg; rbeg != rend; ++rbeg) {
      Input.putback(*rbeg);
    }
  }

  class GenericSelector
  {
  public:

    virtual operator bool() const { return false; }

    virtual bool ParseFromInput(std::istream &Input) = 0;
  };

  inline bool operator >> (std::istream &Input, GenericSelector &Selector)
  {
    return Selector.ParseFromInput(Input);
  }

  class StringParser
  {
  public:

    std::string Text = "";
    operator bool() const { return !Text.empty(); }

    friend bool operator >> (std::istream &Input, StringParser &Parser);
  };

  class TypeSelector : public GenericSelector
  {
  public:

    std::string Text = "";

    operator bool() const override { return !Text.empty(); }

    bool ParseFromInput(std::istream &Input) override final;

  };

  class ClassSelector : public GenericSelector
  {
  public:

    std::string Text = "";

    operator bool() const override { return !Text.empty(); }

    bool ParseFromInput(std::istream &Input) override final;

  };

  class IDSelector : public GenericSelector
  {
  public:

    std::string Text = "";

    operator bool() const override { return !Text.empty(); }

    bool ParseFromInput(std::istream &Input) override final;

  };

  class AttributeSelector : public GenericSelector
  {
  public:

    std::string AttrText = "";
    std::string ValText = "";
    std::string CompText = "";

    operator bool() const override { return !AttrText.empty() && !ValText.empty() && !CompText.empty(); }

    bool ParseFromInput(std::istream &Input) override final;

  };

  class Declaration : public GenericSelector
  {
  public:

    std::string PropertyText = "";
    std::string ValueText = "";

    Declaration() = default;
    Declaration(const Declaration &) = default;
    Declaration(Declaration &&) = default;

    operator bool() const override { return !PropertyText.empty() && !ValueText.empty(); }

    bool ParseFromInput(std::istream &Input) override final;

  };

  class DeclarationBlock : public GenericSelector
  {
  public:

    std::vector<Declaration> Rules;

    operator bool() const override { return !Rules.empty(); }

    bool ParseFromInput(std::istream &Input) override final;

  };

}
