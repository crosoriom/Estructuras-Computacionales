#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

/**
 * @brief Inicializa el contador de ticks del sistema.
 *
 * Esta función toma el argumento y lo carga en el registro
 * de valor de recarga del SysTick para indicar a cuantos
 * ciclos de reloj debe reiniciarse el contador.
 *
 * @param[in] ticks número de ticks para el contador.
 *                  4000 para indicar 1 ms.
 *
 * @warning Esta función configura el SysTick de tal modo que
 * usa el reloj del procesador (AHB).
 * @note La función también activa el System Timer Exception.
 */
void systick_init(uint32_t ticks);

/**
 * @brief Retorna un número de interrupciones.
 *
 * Esta función retorna el número de interrupciones
 * (tiempo transcurrido) que se han lanzado desde el inicio
 * de ejecución o el último reinicio del contador.
 *
 * @return Tick actual.
 */
uint32_t systick_getTick(void);

/**
 * @brief Pone en cero el contador de interrupciones.
 *
 * Esta función restablece el contador de interrupciones
 * (contador de tiempo) del sistema.
 */
void systick_reset(void);

#endif
