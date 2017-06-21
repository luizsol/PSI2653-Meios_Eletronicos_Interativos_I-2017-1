# Exercício Programa 5: Projeto Lumiar

## Descrição

Lumiar é um controlador de luminosidade inteligente baseado no Raspberry Pi 3.


## Estrutura

### main


### web


### ldr
Impedancia do ldr:
* Minima ~ 4 [kohm] (escuro)
* Maxima ~ 65 [ohm] (claridade total)

Como queremos uma constante de tempo `R*C` menor que 200[ms], temos que nosso capacitor nao deve ser maior que 200[ms]/4[kohm] = 50[uF].

Na pior das hipoteses, tambem nao queremos uma constante de tempo menor que 1[ms], portanto queremos um capacitor de capacitancia maior que 1[ms]/65[ohm] = 15.4[uF].

Sendo assim, escolheu-se utilizar 30[uF] de capacitancia no filtro LDRC.

### pwm


### lumiar.ini

Arquivo de configuração.


### queue
