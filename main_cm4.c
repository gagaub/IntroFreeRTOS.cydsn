/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "params.h"
#include "queue.h"

SemaphoreHandle_t bouton_semph ;
int count =2;
bool acquis = false;

void isr_bouton(void)
{
    xSemaphoreGiveFromISR(bouton_semph, NULL);
    Cy_GPIO_ClearInterrupt(Bouton_0_PORT,Bouton_0_NUM);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);
   
}

void bouton_task(void *bouton_semph)
{   
    for(;;)
    {
    acquis = xSemaphoreTake(bouton_semph, pdMS_TO_TICKS(10));
    
    if (count%2 ==0 && acquis == true )
    {
        vTaskDelay(pdMS_TO_TICKS(20));
        UART_1_PutString("\n\r Bouton appuye");
        count++;
        xSemaphoreGive(bouton_semph);
    }
    
    else if (count%2 !=0 && acquis == true )
    {
        vTaskDelay(pdMS_TO_TICKS(20));
        UART_1_PutString("\n\r Bouton relache");
        count++;
        xSemaphoreGive(bouton_semph);
    }
    }
    
  
}

int main(void)

{
    __enable_irq(); /* Enable global interrupts. */
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_1_Start();
    
    
    Cy_SysInt_Init(&Bouton_ISR_cfg, isr_bouton);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);
    
    bouton_semph = xSemaphoreCreateBinary(); // initialis√© avant la prochaine ligne!
    NVIC_EnableIRQ(Bouton_ISR_cfg.intrSrc);
    
    xTaskCreate(bouton_task, "tache_affichage_bouton", 250, NULL, 1, NULL);
    vTaskStartScheduler();
    
    for(;;)
    {
       
    }
}



/* [] END OF FILE */
