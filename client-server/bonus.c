#include <stm32f4xx.h>


int final_number = 0;
void handle_keypad(void);
void check_first();
void check_second();
void check_third();

int main(void){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock for Port A
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // enable clock for Port B
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // enable clock for Port C


    GPIOB->MODER &= ~0x003FFFFF; // Clear mode bits for PB0-10
    GPIOB->MODER |= 0x00155555;  // Set PB0-10 as output (01)

    
}

int handle_keypad(void){
    while (true){
        int current_input = -24;  // set a value for not reading any valid input (negative my student id)
        GPIOC->ODR = 0x1;          // set C1 to check first column
        if((GPIOA->IDR & (0x1)) == 0x1) current_input = 1;
        else if((GPIOA->IDR & (0x2)) == 0x2) current_input = 4;
        else if((GPIOA->IDR & (0x10)) == 0x10) current_input = 7;
        else if((GPIOA->IDR & (0x20)) == 0x20) break; // if pressed star then capture the number

        GPIOC->ODR = 0x2;          // set C2 to check second column
        if((GPIOA->IDR & (0x1)) == 0x1) current_input = 2;
        else if((GPIOA->IDR & (0x2)) == 0x2) current_input = 5;
        else if((GPIOA->IDR & (0x10)) == 0x10) current_input = 8;
        else if((GPIOA->IDR & (0x20)) == 0x20) current_input = 0;

        GPIOC->ODR = 0x4;          // set C3 to check third column
        if((GPIOA->IDR & (0x1)) == 0x01) current_input = 3;
        else if((GPIOA->IDR & (0x10)) == 0x02) current_input = 6;
        else if((GPIOA->IDR & (0x20)) == 0x10) current_input = 9;

        
        if(current_input != -24){
                final_number = final_number * 10 + current_input;
                if(final_number != 0) lcd_print_num(in);                   
        }
    }
    
}


