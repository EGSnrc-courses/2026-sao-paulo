# 1. Primeiros passos: soluções <!-- omit in toc -->

- [1.2. Escrever sua própria aplicação egs++](#12-escrever-sua-própria-aplicação-egs)
- [1.3. Adicionar um objeto de dosimetria](#13-adicionar-um-objeto-de-dosimetria)
- [1.4. Explorar os parâmetros da simulação](#14-explorar-os-parâmetros-da-simulação)
- [1.5. Monitorar uma simulação em detalhe](#15-monitorar-uma-simulação-em-detalhe)

<!-- ## 1.1. -->

## 1.2. Escrever sua própria aplicação egs++

### O `myapp` fornece alguma informação útil ao ser executado?

> Na verdade, não. A aplicação imprime alguns detalhes de configuração e
> estatísticas de tempo, mas o resultado é sempre zero (com 100% de incerteza)
> porque nenhuma grandeza física foi *pontuada* (scored) ainda:
>
> ```text
> Running 1000 histories
>
>   Batch             CPU time        Result   Uncertainty(%)
> ==========================================================
>       1                0.08              0         100.00
>       2                0.16              0         100.00
>       3                0.23              0         100.00
>       4                0.31              0         100.00
>       5                0.40              0         100.00
>       6                0.49              0         100.00
>       7                0.56              0         100.00
>       8                0.64              0         100.00
>       9                0.72              0         100.00
>      10                0.80              0         100.00
>
>
> Finished simulation
>
> Total cpu time for this run:            0.80 (sec.) 0.0002(hours)
> Histories per hour:                     4.5e+06
> Number of random numbers used:          13938479
> Number of electron CH steps:            345747
> Number of all electron steps:           1.2252e+06
> ```

## 1.3. Adicionar um objeto de dosimetria

### Quanta energia é depositada? Qual é a dose?

> Com o objeto de dosimetria, a simulação agora reporta um **Summary of region
> dosimetry**. A energia depositada na placa de tântalo é
> $(2.5506 \pm 1.828\\%)$ MeV por elétron incidente. A dose é
> $(2.4535\times10^{-12} \pm 1.828\\%)$ Gy.
>
> ```text
> ==> Summary of region dosimetry (per particle)
> ir medium      rho/[g/cm3]  V/cm3      Edep/[MeV]              D/[Gy]
> -----------------------------------------------------------------------------
> 1 tantalum  16.654    10.0000 2.5506e+00 +/- 1.828  % 2.4535e-12 +/- 1.828  %
> -----------------------------------------------------------------------------
> ```

### É possível converter manualmente a energia depositada em dose?

> Dose é a energia depositada por unidade de massa, reportada em Gray
> (1 Gy = 1 J/kg). A placa tem densidade 16.654 g/cm³ e volume 10 cm³,
> portanto sua massa é 166.54 g:
>
> $$D = \frac{2.5506 \text{ MeV}}{166.54\text{ g}} = 0.01531 \text{
> MeV/g} = 2.454 \times 10^{-12} \text{ J/kg}$$

### Por que a incerteza relativa é a mesma para energia e dose?

> A incerteza relativa é 1.828% para ambas. A dose é simplesmente a energia
> dividida pela massa, e a massa vem de parâmetros da simulação (densidade e
> volume) que são constantes exatas. Dividir por uma constante não altera a
> incerteza relativa.

### Por que a energia depositada não aumentou por um fator de 10?

> Com `ncase = 1e4`, a energia depositada é $(2.5893 \pm 0.572\\%)$ MeV —
> essencialmente inalterada. Isso ocorre porque os resultados de Monte Carlo são
> reportados *por partícula incidente*, não como totais. A incerteza relativa,
> no entanto, diminuiu por um fator de aproximadamente 3. Isso segue o
> escalonamento $1/\sqrt{N}$ típico de amostragem aleatória:
> $\sqrt{10} \approx 3.2$.

## 1.4. Explorar os parâmetros da simulação

### Cenário A — Fótons em vez de elétrons

**A simulação é mais rápida com elétrons ou com fótons?**

> Definir `charge = 0` muda para fótons. A simulação roda cerca de 10 vezes
> mais rápido porque fótons interagem com a matéria com muito menos frequência
> do que elétrons.

**Como a dose mudou?**

> A dose diminuiu por um fator de aproximadamente 10 em comparação com elétrons,
> consistente com menos interações na placa. A incerteza também é maior, já que
> menos eventos de deposição de energia são amostrados.

**Pósitrons são gerados? Isso é esperado?**

> Sim — eles aparecem como trajetórias azuis no `egs_view`. Isso é esperado:
> os fótons incidentes (20 MeV) estão bem acima do limiar de produção de pares
> de 1.022 MeV.

### Cenário B — Fótons de menor energia

**Qual é a maior diferença qualitativa em comparação com 20 MeV?**

> Há muito menos partículas carregadas secundárias, e nenhum pósitron. Isso
> ocorre porque a energia incidente (1 MeV) está abaixo do limiar de produção
> de pares de 1.022 MeV.

**A dose aumentou ou diminuiu?**

> A dose diminuiu ainda mais, para $(5.509\times 10^{-14} \pm 3.6\\%)$ Gy.

**Como o tempo de simulação mudou?**

> Diminuiu. Menor energia significa menos partículas secundárias para
> transportar, o que torna a simulação mais rápida.

### Cenário C — Elétrons de baixa energia em chumbo

**O que está acontecendo com os elétrons?**

> Os elétrons são absorvidos na placa de chumbo ou retroespalhados na direção
> $-z$.

**Isso é consistente com a energia depositada?**

> Sim. A energia depositada é $(0.0588 \pm 0.71\\%)$ MeV por elétron
> incidente. Como a energia incidente é 0.1 MeV, apenas cerca de 60% da energia
> permanece na placa — o restante é levado pelos elétrons retroespalhados.

**Qual é o tamanho do arquivo `slab.ptracks`?**

> Cerca de 5.3 MB. Arquivos de trajetórias crescem rapidamente, mas por padrão
> o `egs_track_scoring` limita o número de eventos salvos a 1024,
> independentemente do `ncase`.

### Cenário D — Elétrons em água (verificação do poder de freamento)

**A energia depositada é consistente com o poder de freamento?**

> A simulação reporta cerca de 175 keV depositados por elétron. Isso é *menos*
> do que os 245 keV esperados pelo cálculo do poder de freamento
> ($2.454 \times 1.0 \times 0.1 = 0.245$ MeV). A diferença surge porque
> o poder de freamento dá a perda de energia *total*, mas parte dessa energia é
> carregada para fora da placa por partículas secundárias (principalmente fótons
> de bremsstrahlung) em vez de ser depositada localmente.
>
> Essa distinção entre energia *perdida* e energia *depositada* é fundamental em
> dosimetria.

## 1.5. Monitorar uma simulação em detalhe

### Qual é o tipo de interação mais comum?

> O espalhamento de Møller domina (1554 eventos), seguido por bremsstrahlung
> (297) e interações fotoelétricas (184). Há também alguns poucos eventos de
> Rayleigh, Compton e fluorescência.

### Quantos elétrons produzem inicialmente bremsstrahlung vs. um elétron secundário?

> 78 histórias (elétrons incidentes) sofrem inicialmente espalhamento de Møller,
> enquanto 22 sofrem primeiro um evento de bremsstrahlung.

### Qual é o maior número de partículas na pilha ao mesmo tempo?

> O tamanho máximo da pilha é 5, alcançado na história #50.
>
> Ele se acumula através de uma cascata: um evento fotoelétrico produz um
> elétron e um fóton de fluorescência (pilha: 3 elétrons + 1 fóton). Esse fóton
> de fluorescência sofre ele próprio absorção fotoelétrica, produzindo outro
> elétron e outro fóton de fluorescência — levando a pilha a 4 elétrons + 1
> fóton = 5.

### A maioria das partículas é descartada pelo corte de energia ou por sair da geometria?

> O corte de energia é o mecanismo dominante: a maioria das partículas é
> descartada porque sua energia cai abaixo do limiar, em comparação com um
> número relativamente pequeno que sai da geometria.
