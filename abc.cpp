#include <iostream>
#include <math.h>
#include <random>
#include <chrono>
#define BEE_COLONY 10
int count1=0;
#define LIMIT 10*count1/2;
double food_source[5][20];
double a[20][2],a_range[20][2];
using namespace std;
int target1=0;
int target2=0;
int trial_counter[5]={0,0,0,0,0};
double best_sol[20],global_best_sol[20];
double p[5];
double cp[5];
double target_function1(double a[20][2],double food_source1[]);
double fitness_function(double error);
double error1,error2;
double random1()
{
    int i;
    random_device rd;
    mt19937_64 eng(rd());
    uniform_int_distribution<> distr(0,1);
    return distr(eng);
}
void best_source(double food_source[5][20])
{
    int i,key1=0;
    double key=fitness_function(target_function1(a,food_source[0]));
    for(i=1;i<5;i++)
    {
        if(fitness_function(target_function1(a,food_source[i]))>key)
        {
            key=fitness_function(target_function1(a,food_source[i]));
            key1=i;
        }
    }
    for(i=0;i<count1;i++)
    {
        best_sol[i]=food_source[key1][i];
    }
}
void initialize(double a[20][2],double a_range[20][2])
{
    int i=0,j;
   // double r;
    random_device rd;
    mt19937 eng(rd());
    for(i=0;i<5;i++)
    {
        for(j=0;j<count1;j++)
        {
        
            uniform_int_distribution<> distr(a_range[j][0], a_range[j][1]);

            food_source[i][j]=distr(eng);
            if (food_source[i][j]>a_range[j][1]) food_source[i][j]=a_range[j][1];
            else if (food_source[i][j]<a_range[j][0]) food_source[i][j]=a_range[j][0];
        }
    }

}
void update_probability_vector()
{
    double sum=0;
    int i;
    for(i=0;i<5;i++)
    {
        sum=sum+fitness_function(target_function1(a, food_source[i]));
    }
    for (i=0; i<5; i++) {
        p[i]=fitness_function(target_function1(a, food_source[i]))/sum;
    }
    cp[0]=p[0];
    for(i=1;i<5;i++)
    {
        cp[i]=cp[i-1]+p[i];
    }

}
double target_function1(double a[20][2],double food_source1[])
{
    double f1=0,f2=0;
    int i;
    for(i=0;i<count1;i++)
    {
        f1=f1+food_source1[i]*a[i][0];
        f2=f2+food_source1[i]*a[i][1];
    }
   // double error1,error2;
    error1=(double)(target1-f1)/(double)target1;
    error2=(double)(target2-f2)/(double)target2;
    double error;
    error=(0.35*fabs(error1)+0.65*fabs(error2));
    
    return fabs(error);
}
double fitness_function(double error)
{
    return (double)1/(double)(1+error);
}
void send_employee_bees()
{
    int i,j,neighbor,param2change,jump;
    double error,r,solution[count1];
    random_device rd;
    mt19937 eng(rd());
    for(i=0;i<5;i++)
    {
        //random selection of the parameter to change
        uniform_int_distribution<> distr1(0, count1-1);
        param2change=distr1(eng);
        
        //random selection of the neighbor to be selected
        uniform_int_distribution<> distr2(0, 4);
        neighbor=distr2(eng);
        
        while(neighbor==i)
        {
            uniform_int_distribution<> distr2(0, 4);
            neighbor=distr2(eng);
        }
        
        for(j=0;j<count1;j++)solution[j]=food_source[i][j];
        //changing the solution by a small amount
        if(food_source[i][neighbor]-food_source[param2change][neighbor]>0){
            uniform_int_distribution<> distr3(0, (food_source[i][neighbor]-food_source[param2change][neighbor]));
            jump=distr3(eng);
        }
        else{
            uniform_int_distribution<> distr3((food_source[i][neighbor]-food_source[param2change][neighbor]),0);
            jump=distr3(eng);
        }
        solution[neighbor]=food_source[i][neighbor]+jump;
        //rectifying bounds
        if(solution[neighbor]>a_range[neighbor][1])solution[neighbor]=a_range[neighbor][1];
        else if(solution[neighbor]<a_range[neighbor][0])solution[neighbor]=a_range[neighbor][0];
      
               error=target_function1(a,solution);
        //comparing fitness of the modified and previous solution
        if(fitness_function(target_function1(a,food_source[i]))<fitness_function(error))
        {
            for(j=0;j<count1;j++)food_source[i][j]=solution[j];
        }
        else trial_counter[i]++;
    }
    update_probability_vector();
    
}
    
void send_onlooker_bees()
{
    int j,i=0,t=0,param2change,neighbor,jump;
    double error,solution[count1],r;
    random_device rd;
    mt19937 eng(rd());
    

    while(t<5)
    {
        //uniform_real_distribution<double> distr(0.0, 1.0);
        r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
        //select foodsources only on probability basis
        if(r>cp[i])
        {
                       t++;
            uniform_int_distribution<> distr1(0, count1-1);
            param2change=distr1(eng);
            
                      uniform_int_distribution<> distr2(0, 4);
            neighbor=distr2(eng);
            
          
            while(neighbor==i)
            {
                uniform_int_distribution<> distr2(0, 4);
                neighbor=distr2(eng);
            }
            for(j=0;j<count1;j++)solution[j]=food_source[i][j];
            
            if(food_source[i][neighbor]-food_source[param2change][neighbor]>0){
                uniform_int_distribution<> distr3(0, (food_source[i][neighbor]-food_source[param2change][neighbor]));
                jump=distr3(eng);
            }
            else{
                uniform_int_distribution<> distr3((food_source[i][neighbor]-food_source[param2change][neighbor]),0);
                jump=distr3(eng);
            }
            solution[neighbor]=food_source[i][neighbor]+jump;
            //rectifying bounds
            if(solution[neighbor]>a_range[neighbor][1])solution[neighbor]=a_range[neighbor][1];
            else if(solution[neighbor]<a_range[neighbor][0])solution[neighbor]=a_range[neighbor][0];
            error=target_function1(a,solution);
            if(fitness_function(target_function1(a,food_source[i]))<fitness_function(error))
            {
                for(j=0;j<count1;j++)food_source[i][j]=solution[j];
            }
            else trial_counter[i]++;
        }
        i=(i+1)%5;
    }
}
void send_scout_bees()
{
    int i,j,solution[count1],neighbor,jump;
    double r;
    random_device rd;
    mt19937 eng(rd());
    int key=0,flag=0;;
    for(i=1;i<5;i++)
    {
        if(trial_counter[i]>trial_counter[key])key=i;
    }
    for (i=0; i<count1; i++) {
        if(food_source[key][i]==best_sol[i])
        {
            flag=1;
        }
        else{
            flag=0;
        }
    }
        if(trial_counter[key]>5*count1&&flag==0)
        {

            
                for(j=0;j<count1;j++)
                {
                    
                   uniform_int_distribution<> distr(a_range[j][0], a_range[j][1]);
                    food_source[key][j]=distr(eng);
                    if (food_source[key][j]>a_range[j][1]) food_source[key][j]=a_range[j][1];
                    else if (food_source[key][j]<a_range[j][0]) food_source[key][j]=a_range[j][0];
                }
            

        }
    trial_counter[key]=0;
    
}
int main() {
    cout<<"enter number of food items\n";
    
    cin>>count1;
    int i,k,m;
    for(i=0;i<count1;i++)
    {
        cout<<"enter food"<<i<<" protein and calorie content\n";
        cin>>a[i][0]>>a[i][1];
    }
    
    for(i=0;i<count1;i++)
    {
        cout<<"enter range for food"<<i<<endl;
        cin>>a_range[i][0]>>a_range[i][1];
        
    }
    cout<<"enter protein and calorie target\n";
    cin>>target1>>target2;
        for (m=0; m<20; m++) {
            initialize(a, a_range);
            best_source(food_source);
            cout<<"\n initial error is "<<target_function1(a, best_sol)<<endl;
            for(i=0;i<500;i++)
            {
                send_employee_bees();
                send_onlooker_bees();
                best_source(food_source);
                send_scout_bees();
            }
            cout<<"\n error after first 500 iterations is "<<target_function1(a, best_sol)<<endl;
            if(m==0)
            {
                for(k=0;k<count1;k++)global_best_sol[k]=best_sol[k];
            }
            else{
                if(fitness_function(target_function1(a, best_sol))>fitness_function(target_function1(a, global_best_sol)))
                {
                    for(k=0;k<count1;k++)global_best_sol[k]=best_sol[k];
                }
            }
            
            cout<<"\n the global best error is "<<target_function1(a, global_best_sol)<<endl;

        
    }
        cout<<"the sol is\n";
    for (i=0; i<count1; i++) {
        cout<<global_best_sol[i]<<" ";
    }
    cout<<endl<<"error is "<<target_function1(a, global_best_sol)<<endl;
    int protein=0,calories=0;
    for(i=0;i<count1;i++)
    {
        protein=protein+global_best_sol[i]*a[i][0];
        calories=calories+global_best_sol[i]*a[i][1];
    }
    cout<<"the result is "<<protein<<" "<<calories<<endl;
        return 0;
}
