#include <iostream>
using namespace std;

class Vehicle
{
    private:
        string mName;
        int ID;
        int mYear;
        double length;
        double width;
        string mobileNumber;
    public:
        Vehicle()  // default constructor sets initial values to variables
        {
            mName = "";
            ID= 0;
            mYear = 0;
            length = 0;
            width = 0;
            
        }

        void setName(string n) 
        {
            mName = n;
        }   

        void setID(int id)
        {
            ID = id;
        }  

        void setYear(int year)
        {
            mYear = year;
        }  

        void setLength(double l)
        {
            length = l;
        }

        void setWidth(double w)
        {
            width = w;
        }

         void setmNumber(string no)
        {
            mobileNumber=no;
        }

        string getName()
        {
            return mName;
        }

        int getID()
        {
            return ID;
        }

        int getYear()
        {
            return mYear;
        }

        double getLength()
        {
            return length;
        }

        double getWidth()
        {
            return width;
        }

        string getmNumber()
        {
            return mobileNumber;
        }
        

};



class parkingTime
{
    private:
        double tIn = 0;
        double tOut = 0;

    public:
        void setInTime(double in)
        {
            tIn = in;
        }

        void setOutTime(double out)
        {
            tOut = out;
        }

        double getInTime()
        {
            return tIn;
        }

        double getOutTime()
        {
            return tOut;
        }
};



class gSlot
{
    protected:
        double gWidth = 0.0;
        double gLength = 0.0;
        bool empty = true;

    public:
        void setWidth(double x)
        {
            gWidth = x;
        }

        void setLength(double y)
        {
            gLength = y;
        }

        void setEmpty(bool z)
        {
            empty = z;
        }

        double getWidth()
        {
            return gWidth;
        }

        double getLength()
        {
            return gLength;
        }

        bool getEmpty()
        {
            return empty;
        }
};



class Garage
{
    protected:
        gSlot slot[6]; // creating array of slots
    public:

        void setSlotX(int z, gSlot n) // setting given slot to a slot in our array
        {
            slot[z] = n;
        }

        gSlot getSlots(int x) //returns slot at given index
        {
            for(int i = 0 ; i< 6; i++ )
            {
                if(x == i)
                {
                    return slot[i];
                    break;
                }
                
            }
        }
};



class GarageBuilder
{
    protected:
        Garage g;
    public:
        void setGarage(Garage &garage)
        {
            g = garage;
        }

        Garage getGarage()
        {
            return g;
        }

        virtual void buildSlots() = 0;

};



class ConcGarageBuilder:public GarageBuilder
{
    public:
        gSlot s;
        void buildSlots()
        {
            for(int i = 0; i< 6; i++)
            {
                if(i < 3)
                {
                    s.setLength(200);
                    s.setWidth(180);
                    g.setSlotX(i, s);
                }
                else{
                    s.setLength(150);
                    s.setWidth(130);
                    g.setSlotX(i, s);
                }
            }
        }
};



class GarageDirector
{
    private:
        GarageBuilder *b;
    public:
        GarageDirector(){};

        GarageDirector(GarageBuilder * build)
        {
            b = build;
        }

        Garage make()
        {
            b->buildSlots();
            return b->getGarage();
        }
};



class ParkManagement
{

    public:

        void parkIn(double t, parkingTime &T) //sets arrival time of car
        {
            T.setInTime(t);
        }

        void parkOut(double t, gSlot &x, parkingTime &T, Garage &g, int ind) // sets departure time of car and sets slot it was in to empty
        {
            T.setOutTime(t);
            x.setEmpty(true);
            g.setSlotX(ind,x);

        }
};



class SelectBehaviour
{
    public:
        ParkManagement p;
        double time = 0.0;
        virtual void select1(double t , Garage &, Vehicle &, parkingTime &T) = 0;
};



class FirstBehaviour: public SelectBehaviour
{
    public:
        void select1(double t, Garage &g, Vehicle &x, parkingTime &T)
        {
            gSlot tmp;
            for(int i = 0; i < 6; i++)
            {
                tmp = g.getSlots(i);
                //if conditions are met we park in car and set its slot to full
                if(x.getWidth() <= tmp.getWidth() && x.getLength() <= tmp.getLength() && tmp.getEmpty() == true)
                {
                    tmp.setEmpty(false);
                    p.parkIn(t, T);
                    g.setSlotX(i, tmp);
                    break;
                }
                
            }
        }
};


class BestBehaviour: public SelectBehaviour
{
    public:
        void select1(double t, Garage &g, Vehicle &x, parkingTime &T)
        {
            gSlot tmp;
            for(int i = 0; i < 6; i++)
            {
                tmp = g.getSlots(i);
                //if conditions are met we park in car and set its slot to full
                if(x.getWidth() == g.getSlots(i).getWidth() && x.getLength() == g.getSlots(i).getLength() && tmp.getEmpty() == true)
                {
                    tmp.setEmpty(false);
                    p.parkIn(t, T);
                    g.setSlotX(i, tmp);
                    break;
                }
                
            }
        }
};


class SlotSelection //interface
{
    protected:
        SelectBehaviour *sb;

    public:

        virtual void select(double , Garage &, Vehicle &, parkingTime &T) = 0;

};



class FirstFit:public SlotSelection
{
    public:
        void select(double t, Garage &g, Vehicle &x, parkingTime &T)
        {
            sb = new FirstBehaviour();
            sb->select1(t, g, x, T);
        }
};



class BestFit:public SlotSelection
{

    public:
        void select(double t, Garage &g, Vehicle &x, parkingTime &T)
        {
            sb = new BestBehaviour();
            sb->select1(t, g, x, T);
        }

};



class TimeManagement
{
    private:
        double totalT = 0.0, in = 0.0, out = 0.0;
    public:
        double calcTime(parkingTime &s) // calculates total time spent by car and returns it
        {
            in = s.getInTime();
            out = s.getOutTime();
            totalT = out - in;
            return totalT;
        }
};



class Fees //interface
{
    protected:
        double fees = 0.0;
        double tmp = 0.0;

    public:

        virtual double calcFees(Vehicle &, TimeManagement &, parkingTime &) = 0;
        
};



class CalcFees:public Fees
{
    public:

    double calcFees(Vehicle &v, TimeManagement &T, parkingTime &t)
        {
            if(v.getLength() <= 150 ) // if car fits into smaller slot, price will be lower
            {
                tmp = T.calcTime(t);
                fees = tmp * 10.0;
                return fees;
            }
            else if(v.getLength() <= 200 && v.getLength() > 150) // if car fits into bigger slot, price will be higher
            {
                tmp = T.calcTime(t);
                fees = tmp * 15.0;
                return fees;
            }
        }


};



class Income
{
    private:
        double income = 0;
    public:
        void TotalFees(double x) //calculates total income
        {
            income = income + x;
        }

        double getIncome() // return total income
        {
            return income;
        }

};



class TotalCars
{
    private:
        int total;

    public:

        void totalCars(int c) //calculates total cars
        {
            total = c;
        }

        int getTotalCars() // returns total cars
        {
            return total;
        }

    
};



class Display
{
    public:
        void displaySlots(Garage &g) //displays all slots in garage
        {
            gSlot tmp;
            for(int i = 0; i < 6; i++)
            {
                tmp = g.getSlots(i);
                if(tmp.getEmpty())
                {
                    cout << "|0| ";
                    
                }
                else
                {
                    cout << "|x| ";
                }
            }
        }

        void mainDisplay()
        {
            int choice= 0, answer= 0, fit= 0, ind = 0, counter= 0;
            Garage g;
            ConcGarageBuilder build;
            build.setGarage(g);
            ConcGarageBuilder * build1;
            build1 = &build;
            GarageDirector direct(build1);
            g = direct.make();
            
            ParkManagement p;
            TimeManagement t;
            parkingTime b;
            FirstFit x;
            CalcFees f;
            Vehicle v;
            gSlot h;
            Income inc;
            TotalCars k;
            
            double length = 0.0, width = 0.0, time= 0.0, fees = 0.0;
            

            cout << "1. Display available slots. " << endl;
            cout << "2. Park in your car. " << endl;
            cout << "3. Park out and fees. " << endl;
            cout << "4. Total income. " << endl;
            cout << "5. Total cars." << endl;
            cout << "6. Exit." << endl;
            

            while (choice!=6)
                {
                    
                    cout << "\n Please choose an option: " << endl;
                    cin >> choice;
                    switch (choice)
                    {
                    case 1:
                        {
                            displaySlots(g);
                            break;
                        }
                    case 2:
                        {
                            cout << "Enter length of car (150 -> 200): " << endl;
                            cin >> length;
                            v.setLength(length);

                            cout << "Enter width of car (130 -> 180): " << endl;
                            cin >> width;
                            v.setWidth(width);

                            cout <<"Enter 0 for first fit and 1 for best fit: " << endl;
                            cin >> fit;
                            
                            cout << "Enter time of arrival: " << endl;
                            cin >> time;
                            if(fit == 0)
                            {
                                FirstFit f;
                                f.select(time, g, v, b);
                                counter ++;
                                k.totalCars(counter);
                            }
                            else if(fit == 1)
                            {
                                BestFit bf;
                                bf.select(time, g, v, b);
                                counter ++;
                                k.totalCars(counter);
                            }

                            break;
                        }
                    case 3:
                        {   
                            cout << "Enter time of departure: " << endl;
                            cin >> time;
                            cout << "Enter slot that you parked in: " << endl;
                            cin >> ind;
                            p.parkOut(time, h, b, g, ind);
                            fees = f.calcFees(v, t, b);
                            inc.TotalFees(fees);
                            cout << "Your fees: " << fees << endl; 
                            break;
                        }
                    case 4:
                        {
                            cout << "Total income: " << inc.getIncome() << endl;
                            break;
                        }
                    case 5:
                        {
                            cout << "Total number of cars that used the parking lot: " << k.getTotalCars() << endl;
                            break;
                        }
                    case 6:
                        {
                            break;
                        }
                    }
                }
        }



};



int main()
{
    Display d;
    d.mainDisplay();
}



