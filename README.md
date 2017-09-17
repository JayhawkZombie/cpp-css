# cpp-css


### A CSS parser written in C++

---

This is a *minimal* css parsing library.   It is not guarenteed to be fully compliant with the CSS W3C specs (though I will try to achieve this as best I can).  

No, there is no super huge heirarchy of classes.  No, there is no support library this depends upon.  
This library is designed to be easy and logical to use, so it should not require the user to understand every aspect of it.  
It also should not require the user to do a ton of work (still improving this).  

#### Features  
* Type selector support (i.e. ```span```, ```p```, etc)  
* Class selector support (i.e. ```div.value```, ```button.helper```, etc)  
* ID selector support (i.e. ```div#smalldiv```, ```#someotherid```, etc)  
* Attribute selector support (i.e. ```entry[type="csv"]```)  
* (Working on pseudo-class support)  
* Rule declarations (property-value pairs) in the form of ```<PROPERTY>: <VALUE>;```  (eg ```color: blue;```)  

#### Classes  

* GenericSelector - base class for all selectors (including style rules)  
* TypeSelector - for selecting based on types  
* IDSelector - for selecting based on IDs  
* ClassSelector - for selecting based on class  
* Attribute selector - for selecting based on attributes  
* Declaration - for parsing a single rule declaration (eg ```color: blue;```)  
* DeclarationBlock - for parsing entire blocks of rules between braces  

#### Planned Features  
* Automatic selection of user types and  application of style rules via derived class
```cpp
namespace css {
  class Styleable
  {
    virtual std::vector<std::string> Class() const = 0;
    //etc
  }
}
...  
//Somewhere in your code
class MyClass : public css::Styleable
{
public:
  MyClass(...);
  ~MyClass() override;
  std::vector<std::string> Class() const override;
  //etc...
}
    
//Somewhere in code
css::Stylesheet sheet;
sheet.parse(SomeInput);

MyClass myObj;
sheet.Apply(myObj);
```  

* Support for pseudo-classes and pseudo-elements
* Support for hot-reapplication of style w/out re-parsing  
* Support for @rules

#### Tests  
All tests are in Tests.cpp  
[Catch](https://github.com/philsquared/Catch) is used for testing.  
Compile Tests.cpp and execute.  Catch will provie ```main``` for you.  

There are currently 97 assertions in the tests.  
I have, of course, not thought of everything.  More tests will be added as the functionality expands.  

#### Examples of css files that will parse successfully  

**1 style rule generated**
```css 
span { color: red; }
```  

**1 style rule generated**
```css
h1 {  
  border: medium solid black;  
}
```

**2 style rules generated (for 1 selector)**

```css
some-name[randattr~="string"] {
  color: blue;
  float: left;
}
```

**3 style rules generated (for 3 selectors)**  
```css
mydiv button.sometype .class-without-type {
  font-size: 12;
  position: relative;
  border: none;
}
```

Straight out of the tests  

**3 style rules generated (for 5 selectors)**  
``` css
test-simple test.with-class test#with-id test[withattr*=value] .lone-class-without-type #lone-id-without-type
{ 
  simple-prop: value42; 
  another: good-value; 
  yet-another: rgba(0, 1, 1, 1);
}
```  
 
**3 style rules generated (for 5 selectors). 2 ill-formed rules are ignored**  
```css
test-simple test.with-class test#with-id test[withattr*=value] .lone-class-without-type #lone-id-without-type
{ 
  simple-prop: value42; 
  another: good-value; 
  yet-another: rgba(0, 1, 1, 1);
  goodname =badval^ /* Ill-formed, rule ignored */
  float.: left; /* Ill-formed, rule ignored */
}
```

---  
Usage is heavily likely to change soon as generic interfaces are added.  Look in [Tests.cpp](https://github.com/JayhawkZombie/cpp-css/blob/master/cpp-css/Tests.cpp_) for examples
