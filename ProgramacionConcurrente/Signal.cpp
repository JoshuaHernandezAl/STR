//necessary libraries
#include <iostream>
#include <pthread.h>
#include <math.h>
#include <fstream>

//definitions
#define NUM_SIGNALS 4
#define OTHER_THREADS 2
#define cols 4
#define rows 100

using namespace std;

//globals
int state = 1;
/*
    1 = running
    0 = stopped
    2 = paused
*/
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
double sampling[rows][cols];

//Functions prototypes posix

//Signals
void *sin_signal(void *);
void *cos_signal(void *);
void *triangle_signal(void *);
void *square_signal(void *);

// Other fuctions for threads
void *message(void *);
void *write_file(void *);
void *input_keyboard(void *);
void *read_file(void *);




int main(void){
    //response code for threads
    int rc;
    pthread_t signals[NUM_SIGNALS], extra_functions[OTHER_THREADS],td_write_file;;
    for(int i=0;i<OTHER_THREADS;i++){
        if(i==0){
            rc = pthread_create(&extra_functions[i], NULL, message, NULL);
        }
        else if(i==1){
            rc = pthread_create(&extra_functions[i], NULL, input_keyboard, NULL);
        }
        if(rc){
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }
    while(1){
        if(state==1){
            for(int i=0;i<NUM_SIGNALS;i++){
                if(i==0){
                    rc = pthread_create(&signals[i], NULL, sin_signal, NULL);
                }
                else if(i==1){
                    rc = pthread_create(&signals[i], NULL, cos_signal, NULL);
                }
                else if(i==2){
                    rc = pthread_create(&signals[i], NULL, triangle_signal, NULL);
                }
                else if(i==3){
                    rc = pthread_create(&signals[i], NULL, square_signal, NULL);
                }
                if(rc){
                    cout << "Error:unable to create thread," << rc << endl;
                    exit(-1);
                }
            }
            for(int i=0;i<NUM_SIGNALS;i++){
                rc = pthread_join(signals[i], NULL);
            }

            rc = pthread_create(&td_write_file, NULL, write_file, NULL);
            if(rc){
                cout << "Error:unable to create thread," << rc << endl;
                exit(-1);
            }
            pthread_join(td_write_file,NULL);
        }
        
    }

    rc=pthread_join(extra_functions[0],NULL);
    rc=pthread_join(extra_functions[1],NULL);
    
    return 0;
}
//Signals
void *sin_signal(void *){
    if(state==1){
        for(int i=0;i<rows;i++){
            pthread_mutex_lock(&mutex1);
            sampling[i][0] = sin(i*2*M_PI/100);
            pthread_mutex_unlock(&mutex1);
        }
    }
    pthread_exit(NULL);
}
void *cos_signal(void *){
    if(state==1){
        for(int i=0;i<rows;i++){
            pthread_mutex_lock(&mutex1);
            sampling[i][1] = cos(i*2*M_PI/100);
            pthread_mutex_unlock(&mutex1);
        }
    }
    pthread_exit(NULL);
}
void *triangle_signal(void *){
    if(state==1){
        for(int i=0;i<rows;i++){
            if(i<=rows/2){
                pthread_mutex_lock(&mutex1);
                sampling[i][2] = i*0.01;
                pthread_mutex_unlock(&mutex1);
            }
            else{
                pthread_mutex_lock(&mutex1);
                sampling[i][2] = (rows-i)*0.01;
                pthread_mutex_unlock(&mutex1);
            }
        }
    }
    pthread_exit(NULL);
}
void *square_signal(void *){
    if(state==1){
        for(int i=0;i<rows;i++){
            if(i<=rows/2){
                pthread_mutex_lock(&mutex1);
                sampling[i][3] = 0;
                pthread_mutex_unlock(&mutex1);
            }
            else{
                pthread_mutex_lock(&mutex1);
                sampling[i][3] = 1;
                pthread_mutex_unlock(&mutex1);
            }
        }
    }
    pthread_exit(NULL);
}

// Other fuctions for threads
void *message(void *){
    int control = state;
    while(1){
        switch (state)
        {
        case 0:
            if(!state && !control){
                int rc;
                pthread_t td_read_file;
                cout << "\nStopped" << endl;
                cout << "Showing file..." << endl;
                rc = pthread_create(&td_read_file, NULL, read_file, NULL);
                if(rc){
                    cout << "Error:unable to create thread," << rc << endl;
                    exit(-1);
                }
                control = 1;
                pthread_join(td_read_file,NULL);
                exit(0);
            }
            break;
        case 1:
            if(state && control){
                cout << "\nRunning" << endl;
                control = 0;
            }
            break;
        case 2:
            if(state==2 && control==0){
                cout << "\nPaused" << endl;
                control = 1;
            }
            break;
        default:
            cout<<"\nChoice a valid option"<<endl;
            break;
        }
    }
    pthread_exit(NULL);
}
void *input_keyboard(void *){
    char c;
    while(1){
        cout<<"Press 's' to stop, 'p' to pause, 'r' to resume, 'e' to exit"<<endl;
        cin >> c;
        cin.ignore(100,'\n');
        switch (c)
        {
        case 's':
            state = 0;
            break;
        case 'r':
            state = 1;
            break;
        case 'p':
            state = 2;
            break;
        case 'e':
            exit(0);
            break;
        default:
            cout<<"Choice a valid option"<<endl;
            break;
        }
    }
    pthread_exit(NULL);
}
void *write_file(void *){
    ofstream fileSignals("sampling.txt", ios_base::app);
    string sample;
    for(int i=0;i<100;i++){
        if(state==1){
            pthread_mutex_lock(&mutex1);
            sample=to_string(sampling[i][0])+"\t"+to_string(sampling[i][1])+"\t"+to_string(sampling[i][2])+"\t"+to_string(sampling[i][3])+"\n";
            pthread_mutex_unlock(&mutex1);
            fileSignals<<sample;
        }
    }
    fileSignals.close();
    pthread_exit(NULL);
}
void *read_file(void *){
    system("./showFile.sh");
    pthread_exit(NULL);
}
