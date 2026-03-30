# 1. Primeiros passos: rode sua primeira simulação EGSnrc <!-- omit in toc -->


- [1.1. Instalar o EGSnrc](#11-instalar-o-egsnrc)
- [1.2. Escrever sua própria aplicação egs++](#12-escrever-sua-própria-aplicação-egs)
- [1.3. Adicionar um objeto de dosimetria](#13-adicionar-um-objeto-de-dosimetria)
- [1.4. Explorar os parâmetros da simulação](#14-explorar-os-parâmetros-da-simulação)
- [1.5. Monitorar uma simulação em detalhe](#15-monitorar-uma-simulação-em-detalhe)

![cover](../assets/cover.png)

## 1.1. Instalar o EGSnrc

### Baixar e configurar

Abra um terminal e execute os seguintes comandos para baixar o código-fonte do EGSnrc e configurá-lo para este curso:

```bash
cd $HOME                                          # ir para o seu diretório pessoal
git clone https://github.com/nrc-cnrc/EGSnrc.git  # baixar o EGSnrc
cd EGSnrc/                                        # entrar no diretório do EGSnrc
git checkout develop                              # mudar para o branch de desenvolvimento
HEN_HOUSE/scripts/configure.expect course 3       # configurar para este curso, sem compilar aplicações
```

⚠️ O EGSnrc deve ser instalado em um caminho **sem espaços** em nenhum nome de pasta.

<details style="margin-bottom:1em">
<summary style="margin-bottom:0.5em">💡 Novo(a) no terminal?</summary>
<div style="background:#ffeedd;padding:1em 2em 0.2em">

Cada linha acima é um comando. Digite (ou cole) uma linha de cada vez e pressione **Enter**.

- `cd` significa *change directory* — move você para uma pasta
- `$HOME` é uma variável que contém o caminho do seu diretório pessoal (ex.: `/home/student`)
- `git clone` baixa uma cópia do código a partir do GitHub
- `git checkout` muda para uma versão (branch) específica do código

</div>
</details>

### Configurar o ambiente

O EGSnrc precisa de três variáveis de ambiente para funcionar. Para defini-las, abra o arquivo `$HOME/.bashrc` no VS Code a partir do terminal:

```bash
code $HOME/.bashrc &
```

Adicione as três linhas a seguir no final do arquivo:

```bash
export EGS_HOME=$HOME/EGSnrc/egs_home/
export EGS_CONFIG=$HOME/EGSnrc/HEN_HOUSE/specs/course.conf
source $HOME/EGSnrc/HEN_HOUSE/scripts/egsnrc_bashrc_additions
```

Em seguida, recarregue o arquivo na sessão atual do terminal:

```bash
source $HOME/.bashrc
```

<details style="margin-bottom:1em">
<summary style="margin-bottom:0.5em">💡 O que é o <code>.bashrc</code>?</summary>
<div style="background:#ffeedd;padding:1em 2em 0.2em">

O arquivo `$HOME/.bashrc` é executado automaticamente toda vez que você abre um novo terminal. Ele configura o seu ambiente — coisas como caminhos de busca e variáveis. Arquivos que começam com `.` são ocultos por padrão; o VS Code ainda consegue abri-los se você fornecer o caminho completo.

</div>
</details>

<details style="margin-bottom:1em">
<summary style="margin-bottom:0.5em">💡 O que é o caractere <code>&</code> no final de um comando?</summary>
<div style="background:#ffeedd;padding:1em 2em 0.2em">

O `&` no final de um comando executa o programa em segundo plano, permitindo que você continue digitando no terminal.

</div>
</details>

<details style="margin-bottom:1em">
<summary style="margin-bottom:0.5em">🔑 O que significam essas três variáveis?</summary>
<div style="background:#ffeedd;padding:1em 2em 0.2em">


| Variável     | Função                                                                      |
| ------------ | --------------------------------------------------------------------------- |
| `EGS_HOME`   | Sua área de trabalho — onde ficam suas aplicações e arquivos de entrada      |
| `EGS_CONFIG` | Aponta para o arquivo de configuração da sua instalação do EGSnrc           |
| `HEN_HOUSE`  | O diretório de sistema do EGSnrc (definido automaticamente pela terceira linha) |

Você verá essas variáveis ao longo de todo o curso.

</div>
</details>

### Verificar a instalação

Confirme que o ambiente está configurado corretamente:

```bash
echo $EGS_HOME
```

Isso deve imprimir algo como `/home/student/EGSnrc/egs_home/`. Se aparecer uma linha em branco, volte e verifique se você editou o `.bashrc` e executou o comando `source`.

<details style="margin-bottom:1em">
<summary style="margin-bottom:0.5em">💡 Por que o sinal <code>$</code>?</summary>
<div style="background:#ffeedd;padding:1em 2em 0.2em">

O `$` indica ao shell que você quer o *valor* de uma variável, e não o seu nome. Experimente os dois para ver a diferença:

```bash
echo $EGS_HOME     # imprime /home/student/EGSnrc/egs_home/
echo EGS_HOME      # imprime o texto literal EGS_HOME
```

</div>
</details>

### Compilar as interfaces gráficas (GUIs)

O EGSnrc inclui GUIs, em particular o `egs_view` para visualização 3D de geometrias e trajetórias de partículas, e o `egs_gui` para criar dados de materiais. Compile-os agora — você usará o `egs_view` em breve na seção 1.2:

```bash
cd $HEN_HOUSE/egs++/view/       # entrar no diretório do código-fonte do egs_view
make                            # compilar o egs_view (não se preocupe com os avisos)

cd $HEN_HOUSE/gui/egs_gui/      # entrar no diretório do código-fonte do egs_gui
make                            # compilar o egs_gui (não se preocupe com os avisos)
```

<details style="margin-bottom:1em">
<summary style="margin-bottom:0.5em">💡 O que o <code>make</code> faz?</summary>
<div style="background:#ffeedd;padding:1em 2em 0.2em">

`make` lê um arquivo chamado `Makefile` e compila o código-fonte em um programa executável. Você não precisa entender Makefiles neste curso — basta executar `make` quando indicado.

</div>
</details>

## 1.2. Escrever sua própria aplicação egs++

Todas as aplicações EGSnrc devem ficar em uma pasta própria dentro de `$EGS_HOME/`. Crie uma nova pasta chamada `myapp` e entre nela:

```bash
cd $EGS_HOME
mkdir myapp               # criar um novo diretório (mkdir significa: "make directory")
cd myapp
```

Você vai criar **quatro** arquivos nesta pasta: dois arquivos-fonte C++, um Makefile e um arquivo de entrada. Para cada arquivo abaixo, abra o VS Code e use **File → New File** para criá-lo, ou execute `code nomedoarquivo &` no terminal.

### Escrever o código-fonte C++

Crie um arquivo chamado `array_sizes.h` com o seguinte conteúdo. Ele define dois limites de compilação: o número máximo de materiais e o tamanho máximo da pilha de partículas na sua simulação.

```c++
#define MXMED 100
#define MXSTACK 10000
```

Crie um arquivo chamado `myapp.cpp`:

```c++
#include "egs_advanced_application.h"
APP_MAIN (EGS_AdvancedApplication);
```

Essa é a menor aplicação EGSnrc totalmente funcional que se pode escrever! Ela é tão curta porque a biblioteca egs++ faz todo o trabalho de simulação nos bastidores.

### Escrever um Makefile

Crie um arquivo chamado `Makefile` com o seguinte conteúdo padrão. A única linha específica da sua aplicação é o nome `myapp`:

```makefile
include $(EGS_CONFIG)
include $(SPEC_DIR)egspp1.spec
include $(SPEC_DIR)egspp_$(my_machine).conf

USER_CODE = myapp    # o nome da sua aplicação, igual ao nome do diretório

EGS_BASE_APPLICATION = egs_advanced_application
CPP_SOURCES = $(C_ADVANCED_SOURCES)
other_dep_user_code = $(ABS_EGSPP)egs_scoring.h
include $(HEN_HOUSE)makefiles$(DSEP)cpp_makefile
```

### Compilar

Compile a aplicação com `make`, que gera o executável `myapp` em `$EGS_HOME/bin/`:

```bash
make
```

Se você modificar o código-fonte, execute `make` novamente para recompilar.

### Escrever um arquivo de entrada

Para rodar uma simulação, você precisa de um arquivo de entrada que descreva a geometria, os materiais, a fonte de partículas e o que calcular. Você aprenderá a escrevê-los do zero mais adiante no curso, mas por enquanto crie um arquivo chamado `slab.egsinp` com o conteúdo abaixo. Ele modela **mil elétrons de 20 MeV incidentes sobre uma placa de tântalo de 1 mm de espessura.**

Um arquivo de entrada egs++ é organizado em alguns blocos principais:

| Bloco              | Função                                                             |
| :----------------- | :----------------------------------------------------------------- |
| **run control**    | quantas partículas simular                                         |
| **geometry**       | a configuração física (formas, dimensões, regiões)                 |
| **media**          | propriedades dos materiais e energias de corte                     |
| **source**         | o feixe incidente (tipo, energia, direção)                         |
| **ausgab objects** | o que extrair da simulação (doses, trajetórias, ...)               |

Leia o arquivo abaixo e observe os comentários — eles destacam detalhes importantes.

```yaml
################################################################################
#
# Simple slab simulation
#
################################################################################

#===============================================================================
# Run control
#===============================================================================
:start run control:
    ncase = 1000                             # número de partículas incidentes
:stop run control:

#===============================================================================
# Geometry
#===============================================================================
:start geometry definition:

    ### plate
    :start geometry:
        name     = slab
        library  = egs_ndgeometry
        type     = EGS_XYZGeometry
        x-planes = -5, 5                    # em cm
        y-planes = -5, 5                    # em cm
        z-planes = -10, 0, 0.1, 10          # em cm → define 3 regiões:
                                            #   região 0: z = -10 a 0   (vácuo)
                                            #   região 1: z =  0  a 0.1 (placa)
                                            #   região 2: z =  0.1 a 10 (vácuo)
        :start media input:
            media = vacuum tantalum         # define 2 materiais: vacuum (índice 0), tantalum (índice 1)
            set medium = 1 1                # atribui o material 1 (tântalo) à região 1 (a placa)
        :stop media input:
    :stop geometry:

    ### use this geometry for the simulation
    simulation geometry = slab

:stop geometry definition:

#===============================================================================
# Media
#===============================================================================
:start media definition:

    ### cutoff energies
    ae  = 0.521
    ap  = 0.010
    ue  = 50.511
    up  = 50

    ### tantalum
    :start tantalum:
        density correction file = tantalum
    :stop tantalum:

    ### lead
    :start lead:
        density correction file = lead
    :stop lead:

    ### water
    :start water:
        density correction file = water_liquid
    :stop water:

:stop media definition:

#===============================================================================
# Source
#===============================================================================
:start source definition:

    ### pencil beam
    :start source:
        name      = pencil_beam
        library   = egs_parallel_beam
        charge    = -1                      # -1 = elétron, 0 = fóton, 1 = pósitron
        direction = 0 0 1                   # ao longo do eixo z
        :start spectrum:
            type = monoenergetic
            energy = 20                     # em MeV
        :stop spectrum:
        :start shape:
            type     = point
            position = 0 0 -10              # posição inicial em x, y, z (cm)
        :stop shape:
    :stop source:

    ### use this source for the simulation
    simulation source = pencil_beam

:stop source definition:

#===============================================================================
# Viewer control
#===============================================================================
:start view control:
    set color = lead      120 120 200 200
    set color = tantalum  120 255 120 255
    set color = water       0 220 255 200
:stop view control:

#===============================================================================
# Ausgab objects
#===============================================================================
:start ausgab object definition:

    ### particle tracks
    :start ausgab object:
        name    = tracks
        library = egs_track_scoring
    :stop ausgab object:

:stop ausgab object definition:
```

### Executar

Execute a simulação:

```bash
myapp -i slab.egsinp
```

<details style="margin-bottom:1em">
<summary style="margin-bottom:0.5em">💡 O que significa <code>-i</code>?</summary>
<div style="background:#ffeedd;padding:1em 2em 0.2em">

A flag `-i` especifica o arquivo de entrada. Esta é uma convenção comum: a maioria dos programas de linha de comando aceita *flags* (também chamadas de *opções*) que começam com `-` para controlar seu comportamento. Essas opções geralmente são seguidas de um *argumento*, aqui o nome do arquivo `slab.egsinp`.

</div>
</details>

A simulação roda em poucos segundos e imprime os resultados no terminal. Ela também salva as trajetórias das partículas no arquivo `slab.ptracks`.

### Visualizar a geometria e as trajetórias

Abra a geometria e as trajetórias no visualizador 3D:

```bash
egs_view slab.egsinp slab.ptracks
```

No visualizador, marque a caixa **Show tracks** e explore:

| Ação     | Controle                          |
| -------- | --------------------------------- |
| Rotação  | botão esquerdo do mouse           |
| Zoom     | roda de rolagem                   |
| Deslocar | Ctrl + botão esquerdo do mouse    |

Observe a fina placa de tântalo no centro. As trajetórias dos elétrons se espalham ao atravessar a placa, e algumas produzem partículas secundárias emergindo de ambos os lados.

Parabéns — você criou sua primeira aplicação egs++ e rodou sua primeira simulação!

### Pergunta

- Além das trajetórias de partículas, o `myapp` fornece alguma informação útil ao ser executado, como energia depositada, dose ou espectro?

## 1.3. Adicionar um objeto de dosimetria

Por padrão, o `myapp` apenas transporta partículas pela geometria sem reportar nenhuma grandeza de interesse. Para extrair resultados, você adiciona *objetos de pontuação* (scoring objects) ao arquivo de entrada.

Abra o `slab.egsinp` e encontre o bloco `ausgab object definition` no final. Adicione um novo objeto de dosimetria **depois** do objeto de trajetórias existente, **dentro** do bloco `ausgab object definition`. O bloco deve ficar assim:

```yaml
#===============================================================================
# Ausgab objects
#===============================================================================
:start ausgab object definition:

    ### generate particle tracks
    :start ausgab object:
        name    = tracks
        library = egs_track_scoring
    :stop ausgab object:

    ### report dose in region 1 (the plate)
    :start ausgab object:
        name         = dose
        library      = egs_dose_scoring
        dose regions = 1                     # região 1 = a placa de tântalo
        volume       = 10                    # 10 × 10 × 0.1 = 10 cm³
    :stop ausgab object:

:stop ausgab object definition:
```

⚠️ Mantenha o objeto de trajetórias existente — apenas adicione o novo bloco ao lado dele.

<details style="margin-bottom:1em">
<summary style="margin-bottom:0.5em">💡 O que significa "ausgab"?</summary>
<div style="background:#ffeedd;padding:1em 2em 0.2em">

[*Ausgabe*](https://translate.google.com/?sl=de&tl=en&text=ausgabe&op=translate) é uma palavra alemã para "saída" (output). É um termo histórico do código EGS4 original (desenvolvido em parte no DESY, em Hamburgo) que permaneceu no EGSnrc. Um objeto ausgab é simplesmente um módulo que extrai e reporta informações da simulação.

</div>
</details>

Execute a simulação novamente e procure o **Summary of region dosimetry** na saída:

```bash
myapp -i slab.egsinp
```

### Perguntas

- Quanta energia é depositada dentro da placa de tântalo? Qual é a dose?

- Você consegue converter manualmente a energia depositada em dose, considerando que a densidade do tântalo é 16,654 g/cm³?

- Qual é a incerteza relativa na energia e na dose, e por que ela é a mesma para ambas?

- Aumente o `ncase` por um fator de 10 no arquivo de entrada e rode novamente. Por que a energia depositada *não* aumentou por um fator de 10? O que aconteceu com a incerteza?

<details style="margin-bottom:1em">
<summary style="margin-bottom:0.5em">💡 Dica sobre incerteza</summary>
<div style="background:#ffeedd;padding:1em 2em 0.2em">

Pense em como a incerteza estatística de um valor médio escala com o número de amostras $N$.

</div>
</details>

## 1.4. Explorar os parâmetros da simulação

Uma boa maneira de desenvolver intuição sobre o transporte de partículas é mudar uma variável de cada vez e observar o efeito. Em cada cenário abaixo, edite o `slab.egsinp`, rode a simulação e visualize as trajetórias:

```bash
myapp -i slab.egsinp
egs_view slab.egsinp slab.ptracks
```

### Cenário A — Fótons em vez de elétrons

Mude o tipo de partícula incidente para fótons e defina `ncase = 1e4`. Consulte o parâmetro `charge` no bloco de definição da fonte.

**Perguntas:**

- A simulação é mais rápida com elétrons ou com fótons?
- Como a dose mudou em comparação com elétrons?
- Pósitrons são gerados nesta simulação? Isso é esperado a 20 MeV?

### Cenário B — Fótons de menor energia

Mantenha os fótons do cenário anterior, mas mude a energia para 1 MeV.

**Perguntas:**

- Qual é a maior diferença qualitativa nas trajetórias das partículas em comparação com 20 MeV?

<details style="margin-bottom:1em">
<summary style="margin-bottom:0.5em">💡 Dica sobre a diferença qualitativa</summary>
<div style="background:#ffeedd;padding:1em 2em 0.2em">

Procure eventos de produção de pares. Qual é o limiar de energia para a produção de pares?

</div>
</details>

- A dose aumentou ou diminuiu?
- E o tempo de simulação, aumentou ou diminuiu? Por quê?

### Cenário C — Elétrons de baixa energia em chumbo

Redefina a fonte para **elétrons de 100 keV** e mude o material da placa para `lead` (chumbo). Mantenha `ncase = 1e4`.

⚠️ Há três mudanças aqui: tipo de partícula, energia *e* material. Verifique as três antes de rodar.

**Perguntas:**

- O que está acontecendo com os elétrons que atingem a placa de chumbo?
- Isso é consistente com a energia depositada?
- Qual é o tamanho do arquivo `slab.ptracks` em disco?

<details style="margin-bottom:1em">
<summary style="margin-bottom:0.5em">💡 Como verificar o tamanho de um arquivo no terminal</summary>
<div style="background:#ffeedd;padding:1em 2em 0.2em">

```bash
ls -lh slab.ptracks       # -l = listagem detalhada, -h = tamanhos legíveis
```

</div>
</details>

### Cenário D — Elétrons em água (verificação do poder de freamento)

Mude o material da placa para `water` (água) e a fonte para **elétrons de 20 MeV**. Mantenha `ncase = 1e4`.

**Perguntas:**

- O poder de freamento mássico para elétrons de 20 MeV em água líquida é 2,454 MeV·cm²/g. A energia depositada na sua simulação é consistente com esse valor?

<details style="margin-bottom:1em">
<summary style="margin-bottom:0.5em">💡 Dica sobre a verificação do poder de freamento</summary>
<div style="background:#ffeedd;padding:1em 2em 0.2em">

O poder de freamento mássico $S/\rho$ dá a perda de energia por unidade de *densidade superficial* (densidade × espessura). Para uma placa de água de 0,1 cm ($\rho$ = 1,0 g/cm³), esperamos:

$$\Delta E \approx \frac{S}{\rho} \times \rho \times t = 2{,}454 \times 1{,}0 \times 0{,}1 = 0{,}2454 \text{ MeV}$$

Compare com a energia depositada por elétron incidente reportada pela simulação.

</div>
</details>

## 1.5. Monitorar uma simulação em detalhe

A aplicação `tutor4pp` permite acompanhar uma simulação interação por interação. Uma cópia do `slab.egsinp` com `ncase = 1` já está no diretório `tutor4pp`. Compile e execute:

```bash
cd $EGS_HOME/tutor4pp
make
tutor4pp -i slab.egsinp
```

### Lendo a saída

A saída mostra a trajetória de um **único** elétron de 20 MeV atravessando a placa de tântalo de 1 mm. Cada evento é impresso em uma linha com as seguintes colunas:

| Coluna   | Significado                                                       |
| -------- | ----------------------------------------------------------------- |
| `iarg`   | código do tipo de evento                                          |
| `event`  | descrição da interação                                            |
| `NP`     | índice da partícula no topo da pilha                              |
| `charge` | −1 = elétron, 0 = fóton, +1 = pósitron                           |
| `energy` | energia total em MeV (cinética + massa de repouso)               |
| `region` | região da geometria (0, 2 = vácuo; 1 = placa; −1 = escapou)      |
| `x y z`  | posição da partícula em cm                                        |
| `u v w`  | cossenos diretores da velocidade da partícula                     |

O EGSnrc mantém o registro de todas as partículas ativas em uma **pilha** (*stack*): quando uma interação cria partículas secundárias, elas são adicionadas no topo da pilha para serem processadas em seguida. É como uma pilha de panquecas — só que não tão deliciosa. A de cima é consumida primeiro.

No `tutor4pp`, após cada interação, a pilha completa é impressa para que você possa ver todas as partículas em voo (ela é impressa de cima para baixo, então o topo fica na última linha!). Os rótulos `NPold` e `NP` indicam a partícula no topo da pilha antes e depois da interação, respectivamente.

Uma partícula é removida da simulação quando sua energia cai abaixo do limiar de corte (`Energy below AE or AP`) ou quando ela sai da geometria (`User discard`, com `region = -1`).

Leia a saída e acompanhe o que aconteceu: o elétron primário sofre uma série de espalhamentos de Møller (criando elétrons secundários) e alguns eventos de bremsstrahlung (criando fótons). A maioria dos elétrons secundários é imediatamente descartada porque sua energia está abaixo do corte de elétrons `AE`. Os fótons de bremsstrahlung escapam da geometria ou são absorvidos por efeito fotoelétrico, às vezes produzindo fótons de fluorescência. O elétron primário acaba saindo da placa.

### Rodar com mais histórias

Agora mude o `ncase` para 10 no `slab.egsinp` e rode novamente. A saída é longa, então redirecione-a para um arquivo:

```bash
tutor4pp -i slab.egsinp > output.dat
```

<details style="margin-bottom:1em">
<summary style="margin-bottom:0.5em">💡 O que o <code>></code> faz?</summary>
<div style="background:#ffeedd;padding:1em 2em 0.2em">

O operador `>` redireciona a saída do terminal para um arquivo em vez de exibi-la na tela. ⚠️ Se o arquivo já existir, ele será sobrescrito sem nenhum aviso!

</div>
</details>

Você pode abrir `output.dat` no VS Code, ou navegar pelo conteúdo no terminal com o comando `less`:

```bash
less output.dat
```

<details style="margin-bottom:1em">
<summary style="margin-bottom:0.5em">💡 Usando o utilitário <code>less</code></summary>
<div style="background:#ffeedd;padding:1em 2em 0.2em">

- **Espaço** ou **Page Down** — próxima página
- **b** ou **Page Up** — página anterior
- **/** — (barra) buscar texto: digite uma palavra e pressione `Enter`
- **q** — sair

</div>
</details>

### Perguntas

- Qual é o tipo de interação mais comum nesta simulação?

- Quantos elétrons incidentes produzem um fóton de bremsstrahlung inicial vs. um elétron secundário (por espalhamento de Møller)?

- Qual é o maior número de partículas na pilha ao mesmo tempo, e como isso ocorreu?

- A maioria das partículas é descartada porque caiu abaixo de um corte de energia, ou porque saiu da geometria?

<details style="margin-bottom:1em">
<summary style="margin-bottom:0.5em">💡 Dica sobre como contar eventos</summary>
<div style="background:#ffeedd;padding:1em 2em 0.2em">

Você pode listar as ocorrências de um texto específico em um arquivo com `grep`, por exemplo:

```bash
grep "Moller" output.dat
grep "Bremsstrahlung" output.dat
```

Você também pode apenas contá-las com a opção `-c` (count) do `grep`:

```bash
grep -c "Moller" output.dat
grep -c "Bremsstrahlung" output.dat
```

</div>
</details>

---

### [Soluções do laboratório 1](Lab-01-solutions-br-pt.md)
