//Header file class Vector → <vector.hpp>
//><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><
//><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><

  /*$$$$$            /$$                                      /$$$$$$  /$$              
 /$$__  $$          | $$                                     /$$__  $$|__/              
| $$  \__/  /$$$$$$ | $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$ | $$  \__/ /$$ /$$$$$$/$$$$ 
|  $$$$$$  /$$__  $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$|  $$$$$$ | $$| $$_  $$_  $$
 \____  $$| $$  \ $$| $$  \ $$| $$$$$$$$| $$  \__/| $$  \ $$ \____  $$| $$| $$ \ $$ \ $$
 /$$  \ $$| $$  | $$| $$  | $$| $$_____/| $$      | $$  | $$ /$$  \ $$| $$| $$ | $$ | $$
|  $$$$$$/| $$$$$$$/| $$  | $$|  $$$$$$$| $$      |  $$$$$$/|  $$$$$$/| $$| $$ | $$ | $$
 \______/ | $$____/ |__/  |__/ \_______/|__/       \______/  \______/ |__/|__/ |__/ |__/
          | $$                                                                          
          | $$                                                                          
          |_*/
//><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><
//><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><

//><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><
//  Developed by: Yatharth Bhasin       || Licence: *******************
//  Discipline of Physics               || Compile : Ok -> 14/01/2021 12:30 PM
//  IIT Indore                          || Tested : NOK
//  (yatharth1997@gmail.com)            || Documentation : NOK
//  (github: yatharthb97)               ||      
//><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><

//Preprocessor Commands
#pragma once

#include <cmath>
#include <sstream>
#include <iostream>
#include <random>

using namespace std;
//#include"random.h"


#define __VEC_V_TOLERANCE__ 1E-10
#define __VEC_V_UNITTOLERANCE__ 1.0 + __VEC_V_TOLERANCE__
#define __VEC_V_NUNITTOLERANCE__ 1.0 - __VEC_V_UNITTOLERANCE__




/**Class Q - Represents Quartenion. A class of variables that defines a four component Quaternion.
 */
class Q
{

public:
   
   //Class Members 
    double a; /*!< real component of quart */ 
    double b; /*!< imaginary component 1 */
    double c; /*!< imaginary component 2 */
    double d; /*!< imaginary component 3 */
//**********************************************************************************

    //1
    //!Class Constructor. Initialises each component to 0.
    /*
    */
    Q():a(0), b(0), c(0), d(0){} //End of Constructor
//**********************************************************************************

    //2
    //!Overloading Class Constructor. Initialises each component to passed parameters.
    /*
    \param a,b,c,d components
    */
    Q(double a,double b, double c, double d): a(a), b(b), c(c), d(d) 
        {} //End of Overloading Constructor
//**********************************************************************************

    //3
    //!Print Quart Info function. Prints the vector with comma seperated components and enclosed in brackets.
    /*
    */
   std::string info() const 
    {
        std::ostringstream o;
        o << "(" <<a << ", " << b << ", " << c << ", "<< d << ")";
        return o.str();
    } //End of info()
//**********************************************************************************
    //4
    //!Print Quart Raw Info function. Prints the vector with space seperated components and without brackets.
    /*
    */
    std::string infoRaw() const 
    {
        std::ostringstream o;
        o <<a << " " << b << " " << c <<" "<< d;
        return o.str();
    } //End of infoRaw()
//**********************************************************************************

    //Returns unit Q
    Q verson()
    {

    }


    Q rndVerson(const double &r1, const double &r2, const double &r3)
    {}

};//End of class Q
//**********************************************************************************

/** Class V - Represents 3D Vectors. A class of variables that defines a three component Vector.
 */

 //!!!!! --> No spaces   Useful with operator[] -> V vect; 
#define xx 0///////////  vect[xx] vs vect['x']
#define yy 1///////////  vect[yy] vs vect['y']
#define zz 2///////////  vect[zz] vs vect['z']
///////////////////////


class V
{
public:

//Member Variables
	double x; /*!< x-component */
	double y; /*!< y-component */
	double z; /*!< z-component */
    constexpr static double tolerance = __VEC_V_TOLERANCE__; /*!< tolerance limit for the class */
//**********************************************************************************

    //1
    //Default Constructor
    //!Class Constructor. Initialises each component to 0.
    /*
    */
    V():x(0.0), y(0.0), z(0.0)
    {} //End of Default Constructor
//**********************************************************************************
    //2
    //Default Constructor
    //!Class Constructor. Copy a vector(or construt from a vector).
    /*
    \param V &other - const passed by reference that gets copied.
    */
    V(V const &other):x(other.x), y(other.y), z(other.z)
    {} //End of Constructor
//**********************************************************************************

    //3
    //!Overloading Class Constructor. Initialises each component to passed parameters.
    /*
    \param x,y,z components
    */
    V(double x, double y, double z):x(x), y(y), z(z)
    {} //End of Overloading Constructor
//**********************************************************************************

    //4
    //!Print Vector Info function. Prints the vector with comma seperated components and enclosed in brackets.
    /*
    */
    std::string info() const 
    {
        std::ostringstream o;
        o << "(" << x << ", " << y << ", " << z << ")";
        return o.str();
    } //End of info()

    //5
    //!Print Vector Info function seperated by 'sep'. Prints the vector with custom seperator seperated components and enclosed in brackets.
    /*
    */
    std::string info(char sep) const 
    {
        std::ostringstream o;
        o << x << sep << y << sep << z;
        return o.str();
    } //End of info()
//**********************************************************************************
    
    //6
    //!Print Vector raw info function. Prints the vector with space seperated components and without brackets.
    /*
    */
    std::string infoRaw() const 
    {
         std::ostringstream o;
         o <<x << " " << y << " " << z;
         return o.str();
    } //End of infoRaw()
//**********************************************************************************

    //7
    //!Function returns size of the function
    /*
    \return double size
    */
    inline double size() const 
    {
        return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
    } //End of size()


    inline double size_sq() const
    {
        return (this->x*this->x + this->y*this->y + this->z*this->z);
    }
//**********************************************************************************

    //8
    //! Normalise a vector to have unit length. Only normalises if the vector has non-zero length(greater than tolerance limit), else all components are zero.
    /*
    */    
    inline void normalise() 
    {
        double tot = this->size(); //Always positive

            tot = (tot > V::tolerance)*(1.0 / tot);
            x *= tot;
            y *= tot;
            z *= tot;

    } //End of normalise()

    //Alias of normalise()
    void __attribute__((always_inline)) norm() 
    {
        normalise();
    }
//**********************************************************************************

    //9
    //! Checks if the vector is a unit vector. Error tolerance set to __VEC_V_TOLERANCE__
    /*
    \return bool answer
    */
    bool isUnit() const
     {
        if ( this->size() > __VEC_V_NUNITTOLERANCE__ && this->size() < __VEC_V_TOLERANCE__ )
            return false;
        else
            return true;
    } //End of isUnit()
//**********************************************************************************

    //10
    //! Checks if the vector is a unit vector. Error tolerance set to passed tolerance value.
    /*
    \param double tolerance
    \return bool answer
    */
    bool isUnit(double tolerance) const
    {
        if ( this->size() > 1.0 + tolerance && this->size() < 1.0 + tolerance )
            return false;
        else
            return true;
    } //End of isUnit()
//**********************************************************************************

    //11
    //! Returns the negative of the given vector
    inline V neg() const 
    {
        return V(-x, -y, -z);
    } //End of neg()
//**********************************************************************************

    //12
    //! Converts the given vector into its reflection : Negative. Alias "reflect" also defined for this function.
    inline void make_neg()
    {
        this->x = -x;
        this->y = -y;
        this->z = -z;
    } //End of make_neg()

    //Alia "reflect"
    void __attribute__((always_inline)) reflect()
    {
        make_neg();
    }
//**********************************************************************************


    //13
    //! Calculates the dot product between two vectors. Error tolerance set to __VEC_V_TOLERANCE__ .
    /*
    \return double dot product
    */
    inline double dot(const V& other) const
    {
        double temp = x*other.x + y*other.y + z*other.z;
        
        if(temp < __VEC_V_TOLERANCE__ && temp > -1*(__VEC_V_TOLERANCE__)) 
            return 0;
        
        else
            return temp;
    } //End of dot()
//**********************************************************************************
    
    //14
    //! Fast Dot - Calculates the dot product between two vectors. Does not check for tolerance.
    /*
    \return double dot product
    */
    inline double fdot(const V& other) const
    {
        return (x*other.x + y*other.y + z*other.z);
    } //End of dot()
//**********************************************************************************

    //15
    //! Calculates the angle between two vectors using arccos(cos inverse). The block also defines an alias raw_angle for the same function.
    /*
    \return double arc cos
    */
    inline double arccos(const V& other) const
    {
        double temp = x*other.x + y*other.y + z*other.z;
        return acos(temp);
    } //End of arccos()

   //Alias of arccos
    double __attribute__((always_inline)) raw_angle(const V& other) const
   {
        return arccos(other);
   }
//**********************************************************************************

    //16
    //! Cross product of two vectors.
    /*
    \param B V other vector
    \return cross product - V.
    */
    inline V cross(const V& other) const 
    {
        return V(y*other.z - z*other.y, -x*other.z + z*other.x, x*other.y - y*other.x);
    } //End of cross()
//**********************************************************************************
  
    //17
    //! Does uniform scaling of all the components.
    /*
    \param double - scaling constant
    */
    inline void scale(double scale) 
    {
        x = x * scale; y = y * scale, z = z * scale;
    } //End of scale()

//***Templating Functions*********************************************************

    //18
    //! Initalize x, y, and z with the given arguement.
    /*
    \param double initalization value
    */
    inline void xyz(double val)
    {
        x = val; y = val; z = val; 
    }

    //19
    //! Initalize x and y with the given arguement, and z is set to zero.
    /*
    \param double initalization value
    */
    inline void xy(double val)
    {
        x = val; y = val; z = 0.0;
    }

    //20
    //! Initalize y and z with the given arguement, and z is set to zero.
    /*
    \param double initalization value
    */
    inline void yz(double val)
    {
        x = 0.0; y = val; z = val;
    }

    //21
    //! Initalize z and x with the given arguement, and z is set to zero.
    /*
    \param double initalization value
    */
    inline void zx(double val)
    {
        x = val; y = 0.0; z = val;   
    }
//**********************************************************************************


    //22
    //! Multiply corresponding components of a vector.
    inline V compmul(V &other) const
    {
        return V(x*other.x, y*other.y, z*other.z);
    } //End of compmul()
//**********************************************************************************

    //23
    //! Convert the Vector into a null Vector.
    inline void make_null()
    {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
    } //End of null()

    //Alias
    void __attribute__((always_inline)) do_null()
    {
        make_null();
    } //End of null()

//**********************************************************************************

    //24
    //! Checks if the vector is a null vector with set tolerance. Is a size based approach more eficient?
    /*
    \param (optional) tolerance
    \return bool -> true is null
    */
    inline bool is_null(double tolerance = __VEC_V_TOLERANCE__)
    {
        return(x < tolerance && x > -tolerance &&
               y < tolerance && y > -tolerance &&
               z < tolerance && z > -tolerance);
    } //End of is_null()
//**********************************************************************************


//////======================== START OF OPERATORS ==================================
    
    // +, -, *, / return new constructed Vectors.
    //=, +=, -=, *=, /= operate on the existing Vectors.

    //25
    //! Assignement Operator.
    /*
    \param V other - assigner.
    */
    inline void operator= (const V& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    } //End of Operator=
//**********************************************************************************

    //26
    //! Subtraction Operator. Component wise subtraction.
    /*
    \param V other vector.
    \return subracted vector
    */
    inline V operator- (const V& other) const 
    {
        return V(x - other.x, y - other.y, z - other.z);
    } //End of Operator-

//**********************************************************************************

    //27
    //! Addition Operator. Component wise addition.
    /*
    \param V other vector.
    \return added vector
    */
    inline V operator+ (const V &other) const
    {
         return V( x + other.x, y + other.y, z + other.z );
    } //End of Operator+
//**********************************************************************************

    //28
    //! Multiplication Operator. Scales the vector with a scalar.
    /*
    \param scale scalar -> template
    \return V scaled vector
    */
    template <typename T>
    inline V operator* (T scale) const
    {
      return V(x*scale, y*scale, z*scale);     
    } //End of Operator*
//**********************************************************************************

    //29
    //! Division Operator. Scales the vector with a scalar.
    /*
    \param scale scalar -> template
    \return V scaled vector
    */
    template <typename U>
    inline V operator/ (U divscale)
    {
      return V(x/divscale, y/divscale, z/divscale);     
    } //End of Operator/
//**********************************************************************************
   
    //30
    //! Iterative Subtraction Operator.
    /*
    \param V other vector
    */
    inline void operator-= (const V& other) 
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    } //End of operator-= 
//**********************************************************************************

    //31
    //! Iterative Addition Operator.
    /*
    \param V other vector
    */
    inline void operator+= (const V& other) 
    {
            x += other.x;
            y += other.y;
            z += other.z;
    } //End of operator+=
//**********************************************************************************

    //32
    //! Equality Operator with set tolerance.
    /*
    \param other V other vector.
    \return bool comparision result.
    */
    inline bool operator== (const V &other) 
    {
        if((x - other.x) < V::tolerance && (y - other.y) < V::tolerance && (z - other.z) < V::tolerance) 
            return true;

        else 
            return false;

    } //End of operator==
//**********************************************************************************

    //33
    //! Unequality Operator with set tolerance
    /*
    \param o V other vector.
    \return bool comparision result.
    */
    inline bool operator!= (V &other) 
    {
        return !(operator==(other));
    } //End of operator!=
//**********************************************************************************

    //34
    //! Iterative Scaling Operator - double.
    /*
    \param scale double -> template
    */
    template <typename T>
    inline void operator*= (T scale) 
    {
        x *= scale; y *= scale; z *= scale;
    } //End of operator*=
//**********************************************************************************

    //35
    //! Iterative Scaling Division Operator - double.
    /*
    \param scale double -> template
    */
    template <typename T>
    inline void operator/= (T scale) 
    {
        x /= scale; y /= scale; z /= scale;
    } //End of operator/=
//**********************************************************************************

/////END OF OPERATORS /////////////////////////////////////////////////////


/*    //36
    //! Overloaded Array Subindex Operator. Accepts three valid character: 'x', 'y', and 'z', and generates a compiler error if any other character is passed. Correct character param returns the corresponding component of the vector. x → 120, y → 121, z → 122; X → 88, Y → 89, Z → 90
    inline double operator[] (const char* index) const
    {
        if(*index == 'x')
            return this->x;
        else if(*index == 'y')
            return this->y;
        else if(*index == 'z')
            return this->z;
        else
            exit(1);
            //static_assert(false, "Vector V index out of bounds.");
    } //End of operator[]
   // error: invalid conversion from ‘const char*’ to ‘int’ [-fpermissive]
    
    //37
    //! Overloaded Array Subindex Operator for int parameter. Accepts three valid indices: 0, 1, and 2, and generates a compiler error if any other index is passed. Correct index param returns the corresponding component of the vector.
    inline double operator[] (int index) const
    {
        static_assert((index <= 2 && index >= 0), "Vector V index out of bounds.");

        if(index == 0)
            return this->x;
        else if(index == 1)
            return this->y;
        else if(index == 2)
            return this->z;
    } //End of operator[] overloaded*/



//////=========================== END OF OPERATORS ========================================


//-------------------- COMPONENT ACCESSORS --------------------------------------------------
    //38
    //! Accessor for component -> x.
    double __attribute__((always_inline)) X() const
    {
        return this->x;
    } //End of x()

    //39
    //! Accessor for component -> y.
    double __attribute__((always_inline)) Y() const
    {
        return this->y;
    } //End of y()

    //40
    //! Accessor for component -> z.
    double __attribute__((always_inline)) Z() const
    {
        return this->z;
    } //End of z()


    //! Accessor for component -> x squared.
    double __attribute__((always_inline)) X_sq() const
    {
        return this->x*this->x;
    } //End of X_sq()

    //39
    //! Accessor for component -> y squared.
    double __attribute__((always_inline)) Y_sq() const
    {
        return this->y*this->y;
    } //End of Y_sq()

    //40
    //! Accessor for component -> z squared.
    double __attribute__((always_inline)) Z_sq() const
    {
        return this->z*this->z;
    } //End of Z_sq()
//-------------------- COMPONENT ACCESSORS --------------------------------------------------

   /*
    Include code for int functors, double functors and, mt_19937 generator

   */
    //41 - RND
    //! Orthagonalises the given vector to the other vector randomly. Uses random number generation.
    /*
    \param B V other vector
    \param usig_RND_db -> Function pointer that generates unsigned double values
    \return vector A which is orthogonalized
    */
    inline static V rnd_orthogonal_of(const V& B, double(*usig_RND_db)()) 
    {
        //extern int Rndm(int, int);
        V temp;
        temp.x = (0.5 - usig_RND_db())*2;
        temp.y = (0.5 - usig_RND_db())*2;
        temp.z = (-1*temp.x * B.x - temp.y * B.y) / B.z;
        temp.normalise();
        return temp;
    } //End of rnd_orthogonal_of()

    //42 - RND
    //! Orthagonalises the given vector to the other vector randomly. Uses random number generation.
    /*
    \param B V other vector
    \param ar,br -> Two random unsigned double values
    \return vector A which is orthogonalized
    */
    inline static V rnd_orthogonal_of(const V& B, double ar, double br) 
    {
        //extern int Rndm(int, int);
        V temp;
        temp.x = (0.5 - ar)*2;
        temp.y = (0.5 - br)*2;
        temp.z = (-1*temp.x * B.x - temp.y * B.y) / B.z;
        temp.normalise();
        return temp;
    } //End of rnd_orthogonal_of()


    //43 - RND
    //! Orthagonalises the given vector to the other vector randomly. Uses random number generation.
    /*
    \param B V other vector
    \param rndengine -> std::mt19937 object by reference
    \return vector A which is orthogonalized
    */
    inline static V rnd_orthogonal_of(const V& B, std::mt19937 &rndengine) 
    {
        //extern int Rndm(int, int);
        
        V temp;
        temp.x = (0.5 - double(rndengine())/double(rndengine.max()))*2;
        temp.y = (0.5 - double(rndengine())/double(rndengine.max()))*2;
        temp.z = (-1*temp.x * B.x - temp.y * B.y) / B.z;
        temp.normalise();
        return temp;
    } //End of rnd_orthogonal_of()
//**********************************************************************************

    //44 - RND
    //! Converts the given vector to a random unit vector. Uses a function.
    /*
    \param std::mt19937 engine by reference
    */    
    inline void rndUnit(std::mt19937 &rndengine)
    {
        this->x = (0.5 - double(rndengine())/double(rndengine.max()))*2;
        this->y = (0.5 - double(rndengine())/double(rndengine.max()))*2;
        this->z = (0.5 - double(rndengine())/double(rndengine.max()))*2;
        this->normalise();
    } //End of rndUnit()


    //45
    //! Converts the given vector to a random unit vector. Uses a function.
    /*
    \param function pointer usig_RND_db that gives unsigned double values
    */
    inline void rndUnit(double(*usig_RND_db)())
    {
        this->x = (0.5 - usig_RND_db())*2;
        this->y = (0.5 - usig_RND_db())*2;
        this->z = (0.5 - usig_RND_db())*2;
        this->normalise();
    } //End of rndUnit()


    //46
    //! Converts the given vector to a random unit vector. Uses a function.
    /*
    \param a,b,c double values that are unsigned and randomly generated
    */
    inline void rndUnit(double a, double b, double c)
    {
        this->x = (0.5 - a)*2;
        this->y = (0.5 - b)*2;
        this->z = (0.5 - c)*2;
        this->normalise();
    } //End of rndUnit()
//**********************************************************************************
   

///><><><><><><><><><><><>< ROTATION FUNCTIONS ><><><><><><><><><><><><><><><><><><><

    //47
    //! Axis-Angle rotation of vector.
    /*
    \param &Axis V unit axis.
    */
    inline void rotate(const V& Axis, double angle) 
    {
        V axis = Axis; //If Axis is not a unit vector safety.
        if(!axis.isUnit())
        {
            axis.normalise();
        }

        double c,s,c_1;
        c = cos(angle); s = sin(angle); c_1 = 1-c;

        double t1 =  axis.x * axis.x * c_1 + c;
        double t2 =  axis.x * axis.y * c_1 - axis.z * s;
        double t3 =  axis.x * axis.z * c_1 + axis.y * s;
        double t4 =  axis.y * axis.x * c_1 + axis.z * s;
        double t5 =  axis.y * axis.y * c_1 + c;
        double t6 =  axis.y * axis.z * c_1 - axis.x * s;
        double t7 =  axis.z * axis.x * c_1 - axis.y * s;
        double t8 =  axis.z * axis.y * c_1 + axis.x * s;
        double t9 =  axis.z * axis.z * c_1 + c;
            
        double newx = t1*this->x + t2*this->y + t3*this->z;
        double newy = t4*this->x + t5*this->y + t6*this->z;
        double newz = t7*this->x + t8*this->y + t9*this->z;

        this->x = newx;
        this->y = newy;
        this->z = newz;


    } //End of rotate()
//**********************************************************************************
    
    //48
    //! Quarternion rotation method. Overloaded rotate().
    /*
    \param &q Quarternion Q provided for rotation.
    */
    inline void rotate(Q &q) 
    {
        double t2,t3,t4,t5,t6,t7,t8,t9,t10,newx,newy,newz;

            //    t1 = quat.w * quat.w;
            t2 =  q.a * q.b;
            t3 =  q.a * q.c;
            t4 =  q.a * q.d;
            t5 = -q.b * q.b;
            t6 =  q.b * q.c;
            t7 =  q.b * q.d;
            t8 = -q.c * q.c;
            t9 =  q.c * q.d;
            t10 = -q.d * q.d;

            newx = 2.0 * ( (t8+t10) * x + (t6-t4)  * y + (t3+t7) * z ) + x;
            newy = 2.0 * ( (t4+t6)  * x + (t5+t10) * y + (t9-t2) * z ) + y;
            newz = 2.0 * ( (t7-t3)  * x + (t2+t9)  * y + (t5+t8) * z ) + z;

            x = newx;
            y = newy;
            z = newz;
    } //End of rotate() Overloaded
//**********************************************************************************

//END
///><><><><><><><><><><><>< ROTATION FUNCTIONS ><><><><><><><><><><><><><><><><><><><


    //49
    bool inline is_ortogonal(V &other)
    {

    }

    //50
    bool inline is_ortogonal(V &other, double tolerance)
    {
        
    }


    //51
    //! Returns a unit vector that points from the initial to the terminal point. Point vector = (terminal point) - (initial point) . {{ initial point --> terminal point }}
    /*
    \param other V other vector - terminal point.
    \return point unit vector
    */
    inline V points_to(V const &other) const //say "points from 'this->' to 'other'."
    {
            V temp;
            temp.x = other.x - x;
            temp.y = other.y - y;
            temp.z = other.z - z;
            temp.normalise();
            return temp;
    } //End of point()
//**********************************************************************************
    //52
    //! Reurns the largest component of the three. 0:x, 1:y, 2:z. If there is a tie, the first element is returned.
    /*
    \param &vec V vector
    \return unsigned int component
    */
/*    inline unsigned int max_component(V const &vec)
    {
        int max = 0;
            
        if(vec.y > vec.x)
             max = 1;
        if(vec.z > vec[max])
            max = 2;

        return max;
    } //End of max_component()
//**********************************************************************************

    //53
    //! Reurns the largest component of the three. 0:x, 1:y, 2:z. If there is a tie, the first element is returned.
    /*
    \param &vec V vector
    \return unsigned int component
    */
/*    inline unsigned int min_component(V const &vec)
    {
        int min = 0;
            
        if(vec.y < vec.x)
             min = 1;
        if(vec.z < vec[min])
            min = 2;

        return min;
    } //End of max_component()*/
//**********************************************************************************

    //54
    //! Returns the projection of vec on the plane with norm planenorm.
    /*
    \param &vec V vector
    \param &planenorm V normal to the given plane
    */
    V static plane_projection(const V &vec, const V &planenorm) //Vec must be terminating on the plane.
    {
        V tempplanenorm = planenorm;
        V temp = tempplanenorm*(planenorm.dot(vec)); //Find projection of vec with the normal
        return V(vec - temp); //return the other component

    } //End of plane_projection()

    //55
    //! Returns the projection of vec on the plane defined by the other two vectors.
    /*
    \param &vec V vector
    \param &planevec1, &planevec2 are vectors that reside in the said plane
    */
    V static plane_projection(const V &vec, const V &planevec1, const V &planevec2)
    {
        V planenorm = planevec1.cross(planevec2); //Find the normal using the two vectors
        V temp = planenorm*planenorm.dot(vec); //Find projection of vec with the normal
        return V(vec - temp); //return the other component

    } //End of plane_projection() Overloaded


    //56
    //! Returns the projection of vec parallel to the given plane.
    /*
    \param &vec V vector
    \param &planenorm V normal to the given plane
    */
    V static plane_parallelprojection(const V &vec, const V &planenorm) //Vec must be terminating on the plane.
    {
        V tempplanenorm = planenorm;
        V temp = tempplanenorm*(planenorm.dot(vec)); //Find projection of vec with the normal
        return temp; //return the projection

    } //End of plane_parallelprojection()



    //57
    //! Returns the size of the corresponding linesegment
    /*
    \param other V& other
    \return line segment length as double
    */
    inline double segment_len(const V &other) const
    {
        V temp(x - other.x, y - other.y, z - other.z);
        return temp.size();
    }

    //58
    //! Checks if one or more components is NaN. Returns true if any of the one component is NaN.   
    inline bool is_nan() const
    {
        if(std::isnan(x) || std::isnan(y) || std::isnan(z))
            return true;
        else
            return false;
    } //End of is_nan


    //59
    inline double Scalar_tri_prod(const V &A, const V &B, const V &C)
    {
        //Scalar triple product: A • (B x C)
        V tmp = B.cross(C);
        return A.dot(tmp);
    } // End of scalar_tri_prod()

    //60
    inline V Vector_tri_prod(const V &A, const V &B, const V &C)
    {
        //Vector triple product: A x (B x C) = B * A•C - C * A•B
        return (B * (A.dot(C))) - (C * A.dot(B));
    } // End of vector_tri_prod()


//Scalar functions
    inline void setscalar(double scalar)
    {
        this->x = scalar;
        this->y = 0.0;
        this->z = 0.0;
    }

    inline bool is_scalar() const
    {
        return std::fabs(this->x) > V::tolerance && 
               std::fabs(this->y) < V::tolerance && 
               std::fabs(this->y) < V::tolerance;
    }

    inline double getscalar() const
    {
        return X();
    }

    
    inline void comp_divide(const V &other)
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;
    }



    //How to represent Infinitesimal Vector?





    //Multi Vector Operations


    //Friend Declarations
    friend std::ostream& operator<< (std::ostream &stream, const V &vec); //61

}; //end of class V
//**********************************************************************************
 
//61 
//! Stream operator overload that puts V::info() to the ostream.
std::ostream& operator<< (std::ostream &stream, const V &vect)
{
    stream << vect.info();
    return stream;
} //End of friend overload operator<<

////////////////////////////////END OF TRANSLATION UNIT/////////////////////////


/*   N   O   T   E   S
    
    • Typically, we won’t be able to use a member overload if the left operand is either not a class (e.g. int), or it is a class that we can’t modify (e.g. std::ostream).

    • The normal or friend function version has the added benefit of “symmetry”, as all operands become explicit parameters (instead of the left operand becoming *this and the right operand becoming an explicit parameter).

*/


//////|||||||||||||||||||||||||| NEW UPCOMING FEATURES|||||||||||||

  /*  bool linearly_dependent(const V &A, const V &B, const V &C)
    {
        double x_indices[3];
        double y_indices[3];
        double z_indices[3];

        double coeff[3] = {0.0};        
    }*/

    /*Orthonormal_Set()
    {
        //Inner product is dirac_delta()


    }


    Make Variadic
    Gen_Matrix_3XN<N>()
    {

    }

    Gen_Matrix_NX3<N>()
    {
        
    }

    Direct_Sum(const V &A, const V &B)

    Direct_Product(const V &A, const V &B)


    //Derivative and Integration si wrt to all the elements of matrix individually.

    //***
    //Returns the inverse of vector such that {{ this->V * returned V = I (Identity) }}
    V const Inverse() //==> 
    {
        V temp = ()
        
    }*/

//////|||||||||||||||||||||||||| NEW UPCOMING FEATURES|||||||||||||