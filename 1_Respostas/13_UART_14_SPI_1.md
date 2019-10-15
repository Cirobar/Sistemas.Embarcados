1. Cite as vantagens e desvantagens das comunicação serial:

(a) Assíncrona (UART).

> A comunicação UART permite receber e enviar dados de maneira assíncrona entre 2 dispositivos. Para elevadas taxas de transferência a sincronia entre _TX_ e _RX_ pode ser perdida já que não há uma linha de clock comum entre os 2 dispositivos.

(b) SPI.

> A comunicação SPI permite o esquema _Full-Duplex_, ou seja, toda troca de dados acontece sempre em ambas as direções, cada bit trocado entre o _master_ e um _slave_ trás um bit do _slave_ para o _master_. Possui pinos de transmissão e recepção, porém, podem ser necessários mais pinos para realizar o controle da seleção de chip _CS_. Também deve ser desenolvolvido um protocolo para que a troca de dados entre _master_ e _slave_ seja compreensível para ambos.

2. Considere o caso em que a Raspberry Pi deve receber leituras analógico/digitais de um MSP430, e que a comunicação entre os dois é UART. É tecnicamente possível que o MSP430 mande os resultados da conversão A/D a qualquer hora, ou ele deve aguardar a Raspberry Pi fazer um pedido ao MSP430? Por quê?

> A comunicação UART é assíncrona e permite com que os dados sejam enviados e recebidos de maneira simultânea, cabendo aos dispositivos envolvidos interpretar os dados recebidos e enviados.

3. Considere o caso em que a Raspberry Pi deve receber leituras analógico/digitais de um MSP430, que a comunicação entre os dois seja SPI, e que o MSP430 seja o escravo. É tecnicamente possível que o MSP430 mande os resultados da conversão A/D a qualquer hora, ou ele deve aguardar a Raspberry Pi fazer um pedido ao MSP430? Por quê?

> Dada a característica _Full-Duplex_ da comunicação SPI é possível enviar e receber dados a qualquer momento já que o _master_ e o _slave_ trocam dados entre si de maneira circular.

4. Se o Raspberry Pi tiver de se comunicar com dois dispositivos via UART, como executar a comunicação com o segundo dispositivo?

> Neste caso a comunicação _UART_ envia um byte adicional para o endereço do dispositivo antes de enviar os dados necessários.

5. Se o Raspberry Pi tiver de se comunicar com dois dispositivos via SPI, como executar a comunicação com o segundo dispositivo?

> Pode ser utilizado um esquema _Daisy Chain_ no qual os _slaves_ estão conectados entre si pelos pinos MISO (slave1) e MOSI(slave2), sendo o pino MISO do _slave2_ conectado ao MOSI do _master_ formando uma cadeia. Nesse caso, o _master_ requer 2 ciclos de leituras para ler os dados referentes aos 2 dispositivos. Ainda no esquema _Daisy Chain_ não é preciso utilizar mais de um _CS (Chip Select)_ para controlar cada um dos _slaves_. Neste esquema de ligação regular com mais de uma saída _CS_ os _slaves_ compartilham _MISO_ com o _master_.
