1. O que são sistemas embarcados?
São sistemas que combinam hardware e software para executar uma função específica juntamente de atuadores e sensores, podendo estes serem mecânicos e/ou elétricos. De maneira geral, os sistemas embarcados podem fazer parte um produto ou sistema maior.

2. O que são sistemas microprocesssados?
São sistemas que apresentam uma arquitetura de hardware com microprocessador. Essa nomenclatura (microprocessador) é dada para o dispositivo que desempenha a função é de desempenhar o ciclo *"fetch-decode-execute"* requerendo dispositivos externos de interface (memória, barramento e afins).

3. Apresente aplicações de sistemas embarcados:
	(a) Para a indústria automotiva;
		Sistema de controle de freios ABS de um veiculo.
	(b) Para eletrodomésticos;
		Sistema para controlar temperatura de secagem de uma máquina de secar roupas.
	(c) Para automação industrial.
		Sistema de controle de esteira de uma linha de produção.

4. Cite arquiteturas possíveis e as diferenças entre elas.
* Microcontroladores: São dispositivos de menor poder computacional que dispõe de toda a arquitetura de maneira integrada para realizar sua função principal.
* Processadores de Propósito Geral - *GPPs*: São dispositivos microprocessados fabricados para aplicações de uso geral.
* *Digital Signal Processors - DSPs*: São dispositivos que apresentam uma arquitetura específica para processar sinais digitais podendo haver hardware adicional para realizar multiplicações e divisões bem como entradas e saídas voltadas para processar sinais.
* *Field Programmable Gate Arrays - FPGAs*: São dispositivos voltados para a implementação de portas lógicas em uma arquitetura que permite a reconfigurabilidade do hardware para diversas finalidades. Apresentam baixo consumo energético se comparado aos GPPs.
* *Application Specific Integrated Circuits - ASICs*: São dispositivos que apresentam o uso de circuitos integrados projetados com finalidades específicas para atender uma necessidade de projeto.
* *System-on-Chip - SoCs*: É um dispositivo que combina as arquiteturas citada com GPPs para atender um requisito de projeto e flexibilizar seu uso.
* *Multiple Processor System-on-Chip - MPSoCs*: São SoCs que utilizam mais de um microprocesador para garantir alto poder computacional.

5. Por que usamos o Raspberry Pi na disciplina, ao invés de outro system-on-chip?
A Raspberry Pi (RPi) é utilizada pois apresenta grande documentação, tem um ciclo de vida bem estabelecido e utiliza distribuições e kernel do Linux que também é um sistema operacional bem documentado e maduro. Além disso há uma grande aproximação da RPi aos projetos que podem ser desenvolvidos na presente disciplina. As funções de GPIO da RPi são bastante vantajosas nesse aspecto.

