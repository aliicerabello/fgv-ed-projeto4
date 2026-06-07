````md
# Trabalho 4: Sistema de Autocomplete de Jogos com Trie

![C++](https://img.shields.io/badge/language-C++-blue)
![Build](https://img.shields.io/badge/build-passing-brightgreen)
![Status](https://img.shields.io/badge/status-academic--project-lightgrey)

Sistema de autocomplete de jogos desenvolvido em C++, utilizando uma estrutura de dados Trie para armazenar títulos e realizar buscas por prefixo.

---

## Descrição

Este projeto implementa um **sistema de autocomplete de jogos**.

O sistema armazena uma base de jogos e permite buscar sugestões a partir de um prefixo informado pelo usuário. Cada jogo possui:

- título
- descrição curta
- popularidade

Dado um prefixo e um valor `k`, o sistema retorna até `k` jogos cujos títulos começam com o prefixo informado.

As sugestões são ordenadas de acordo com os seguintes critérios:

1. maior popularidade primeiro;
2. em caso de empate, ordem alfabética pela chave de busca do título.

O projeto utiliza uma estrutura de dados chamada **Trie**, também conhecida como árvore de prefixos. Essa estrutura armazena strings caractere por caractere e é especialmente adequada para operações de busca por prefixo.

---

## Organização Interna dos Dados

Os dados principais do projeto são representados pelas classes `Game`, `Trie` e `TrieNode`.

Cada jogo é representado pela classe `Game`.

A estrutura `Trie` armazena os títulos dos jogos usando uma árvore de prefixos. Cada nó da Trie é representado por um `TrieNode`.

### Estrutura geral

```text
Trie
│
└── root
    │
    ├── TrieNode
    │   ├── children[]
    │   ├── isEndOfTitle
    │   └── game
    │
    ├── TrieNode
    │   ├── children[]
    │   ├── isEndOfTitle
    │   └── game
    │
    └── ...
```

Cada `TrieNode` armazena:

- um array de ponteiros para os filhos;
- uma marcação indicando se o nó representa o fim de um título;
- um ponteiro para o jogo associado ao título, quando existir.

A classe `Game` armazena:

- título;
- descrição curta;
- popularidade.

A base inicial de jogos é fornecida nos arquivos `GamesDatabase.hpp` e `GamesDatabase.cpp`.

---

## Funcionamento

### Inserção de jogos (`insert`)

Quando um jogo é inserido na Trie:

1. o título do jogo é convertido para uma chave de busca;
2. a Trie é percorrida caractere por caractere;
3. nós são criados quando ainda não existe caminho para determinado caractere;
4. ao final do título, o último nó é marcado como fim de título;
5. o ponteiro para o jogo é armazenado nesse último nó.

A inserção recebe um ponteiro para `Game`, pois os jogos já existem na base carregada pelo programa.

---

### Busca por título exato (`contains`)

O método `contains` verifica se um título está armazenado na Trie.

Antes da busca, o título é convertido para sua chave de busca. Assim, buscas com letras maiúsculas, minúsculas ou espaços diferentes devem ser tratadas de forma equivalente.

Por exemplo, as buscas abaixo representam o mesmo título:

```cpp
contains("Half Life");
contains("halflife");
contains("HALF LIFE");
```

A busca percorre a Trie caractere por caractere. Ao final, o método retorna `true` apenas se o último nó encontrado estiver marcado como fim de título.

---

### Chave de busca (`toSearchKey`)

Antes de inserir ou buscar um título, o texto é normalizado pelo método `toSearchKey`.

A normalização adotada neste projeto segue as regras:

- letras maiúsculas são convertidas para minúsculas;
- espaços em branco são ignorados;
- números são preservados;
- os títulos são considerados compostos apenas por letras, números e espaços.

Exemplos:

```cpp
toSearchKey("Hades") == "hades"
toSearchKey("HA") == "ha"
toSearchKey("Half Life") == "halflife"
toSearchKey("Portal 2") == "portal2"
```

Essa chave é a representação usada internamente pela Trie.

---

### Mapeamento de caracteres (`indexCalculator`)

A Trie utiliza um array de filhos em cada nó. Para acessar a posição correta desse array, cada caractere da chave de busca é convertido para um índice inteiro.

A estratégia adotada foi:

- letras de `'a'` até `'z'` ocupam os índices de `0` a `25`;
- dígitos de `'0'` até `'9'` ocupam os índices de `26` a `35`.

Assim, o alfabeto interno da Trie possui 36 posições:

```text
26 letras + 10 dígitos = 36 caracteres
```

Caso algum caractere não pertença ao conjunto esperado, o método retorna `-1`.

---

## Autocomplete

O método `autocomplete` recebe:

- um prefixo;
- um número máximo de resultados `k`.

O funcionamento é:

1. se `k <= 0`, retorna um vetor vazio;
2. o prefixo é convertido para chave de busca;
3. a Trie é percorrida até o nó correspondente ao fim do prefixo;
4. se o prefixo não existir, retorna um vetor vazio;
5. a partir do nó encontrado, os jogos abaixo dele são coletados;
6. durante a coleta, o vetor de resultados é mantido com no máximo `k` jogos;
7. o vetor final é retornado.

Dessa forma, o método não precisa coletar todos os jogos encontrados para só depois cortar o vetor. A seleção dos melhores `k` resultados já acontece durante a chamada de `collectGames`.

---

### Coleta de jogos (`collectGames`)

Após encontrar o nó correspondente ao prefixo, o método `collectGames` percorre recursivamente todos os nós abaixo dele.

Sempre que encontra um nó marcado como fim de título e com jogo associado:

1. adiciona o jogo ao vetor de resultados;
2. ordena o vetor de acordo com o critério do autocomplete;
3. se o vetor ultrapassar tamanho `k`, remove o último elemento.

Como o vetor é mantido ordenado, o último elemento é sempre o pior entre os resultados guardados até aquele momento.

Assim, ao longo da coleta, o algoritmo mantém apenas os melhores `k` jogos encontrados até então.

---

## Ordenação dos Resultados

Os resultados do autocomplete são ordenados pelo método `sortResults`.

O critério de ordenação é:

```text
maior popularidade vem primeiro
empate → menor chave de busca em ordem alfabética vem primeiro
```

Ou seja, jogos mais populares aparecem antes. Caso dois jogos tenham a mesma popularidade, o desempate é feito pela chave normalizada do título.

Por exemplo, se os jogos `Halo` e `Half Life` tiverem a mesma popularidade, `Half Life` vem antes, pois:

```text
halflife < halo
```

---

### Insertion Sort

A ordenação dos resultados foi implementada manualmente usando Insertion Sort.

O algoritmo percorre o vetor de jogos e insere cada elemento na posição correta em relação aos anteriores.

Principais características:

- implementação simples;
- adequada para listas pequenas ou moderadas de sugestões;
- não utiliza `std::sort`;
- respeita o critério de popularidade e desempate alfabético;
- possui custo quadrático no caso geral.

Complexidade:

| Caso | Complexidade |
|------|-------------|
| Melhor caso | O(n) |
| Caso médio | O(n²) |
| Pior caso | O(n²) |

No autocomplete, como o vetor de resultados é mantido com no máximo `k` elementos, a ordenação durante a coleta é feita sobre um vetor pequeno, limitado pelo valor de `k`.

---

## Operações

As principais operações implementadas no projeto são:

- criação de jogos (`Game`);
- acesso ao título do jogo (`getTitle`);
- acesso à descrição curta (`getShortDescription`);
- acesso à popularidade (`getPopularity`);
- criação da Trie (`Trie`);
- liberação da memória da Trie;
- inserção de jogos na Trie (`insert`);
- busca por título exato (`contains`);
- conversão para chave de busca (`toSearchKey`);
- conversão de caractere para índice (`indexCalculator`);
- autocomplete por prefixo (`autocomplete`);
- coleta recursiva de jogos (`collectGames`);
- comparação entre jogos (`comesBefore`);
- ordenação dos resultados (`sortResults`).

---

## Complexidade

| Operação | Complexidade |
|----------|-------------|
| `toSearchKey(text)` | O(t) |
| `indexCalculator(c)` | O(1) |
| `insert(game)` | O(t) |
| `contains(title)` | O(t) |
| `sortResults(results)` | O(m²) |
| `collectGames(node, results, k)` | O(s + r · k²) |
| `autocomplete(prefix, k)` | O(p + s + r · k²) |
| Destrutor da Trie | O(N) |

Onde:

- `t` é o tamanho do título normalizado;
- `p` é o tamanho do prefixo normalizado;
- `s` é o número de nós percorridos na subárvore do prefixo;
- `r` é o número de jogos encontrados na subárvore do prefixo;
- `k` é o número máximo de sugestões solicitado;
- `m` é o tamanho do vetor passado para ordenação;
- `N` é o número total de nós da Trie.

A operação de autocomplete envolve encontrar o nó correspondente ao prefixo e percorrer a subárvore abaixo dele. Durante essa coleta, os resultados são mantidos limitados a `k` elementos.

---

## Detalhes de Implementação

- Implementação em C++;
- uso de classes e modularização com arquivos `.hpp` e `.cpp`;
- armazenamento dos títulos em uma Trie;
- cada nó possui um array de filhos;
- cada fim de título armazena um ponteiro para o jogo correspondente;
- busca case-insensitive;
- espaços são ignorados nas buscas;
- dígitos são preservados na chave de busca;
- alfabeto interno composto por letras minúsculas e dígitos;
- ordenação manual com Insertion Sort;
- uso de `std::vector<Game*>` para armazenar os resultados encontrados;
- o vetor de resultados do autocomplete é mantido com no máximo `k` elementos;
- controle de memória feito no destrutor da Trie e dos nós;
- a base de jogos é carregada diretamente do código-fonte;
- não há leitura de arquivos em tempo de execução;
- não há menu interativo.

---

## Alternativas Consideradas

- Busca linear na base de jogos  
  - implementação mais simples;
  - exigiria verificar todos os títulos a cada busca;
  - menos adequada para buscas por prefixo.

- Array ou vetor ordenado por título  
  - poderia facilitar algumas buscas;
  - exigiria estratégia adicional para encontrar intervalos de prefixo;
  - inserções seriam mais custosas se a estrutura fosse mantida ordenada.

- Trie com `map` ou `unordered_map` nos filhos  
  - economizaria espaço quando poucos filhos existissem;
  - adicionaria dependência de estruturas prontas da STL;
  - tornaria o acesso aos filhos diferente da proposta baseada em array.

- Coletar todos os jogos antes de selecionar os `k` melhores  
  - implementação simples;
  - pode consumir mais memória quando muitos jogos possuem o mesmo prefixo;
  - foi substituída por uma coleta que mantém apenas os melhores `k` resultados.

- Uso de `std::sort` para ordenar resultados  
  - simplificaria a implementação;
  - não exercitaria a implementação manual de algoritmo de ordenação;
  - foi substituído por Insertion Sort implementado manualmente.

---

## Trade-offs

- A Trie é eficiente para encontrar rapidamente o nó correspondente a um prefixo;
- o uso de array de filhos torna o acesso a cada caractere simples e direto;
- o array de filhos pode desperdiçar memória quando muitos nós possuem poucos filhos;
- a normalização simplifica buscas case-insensitive e com espaços;
- guardar ponteiros para `Game` evita copiar objetos grandes durante a inserção;
- a coleta recursiva é simples, mas pode percorrer muitos nós se o prefixo for muito amplo;
- manter apenas os melhores `k` resultados reduz o tamanho do vetor armazenado durante o autocomplete;
- ordenar o vetor a cada novo jogo encontrado é simples, mas adiciona custo durante a coleta;
- o Insertion Sort é simples, mas pode ser custoso se `k` for grande;
- o uso de ponteiros exige cuidado com gerenciamento de memória;
- a Trie não é responsável por destruir os objetos `Game`, apenas os nós criados internamente.

---

## Testes

O projeto pode ser testado usando o arquivo `main.cpp`.

Os testes principais envolvem:

- carregar todos os jogos da base;
- inserir os jogos na Trie;
- buscar títulos existentes com `contains`;
- verificar buscas case-insensitive;
- verificar buscas ignorando espaços;
- executar autocomplete com diferentes prefixos;
- testar prefixos inexistentes;
- testar valores de `k`;
- verificar ordenação por popularidade;
- verificar desempate por ordem alfabética da chave de busca;
- verificar que o autocomplete retorna no máximo `k` resultados.

Exemplos de prefixos úteis para teste:

```bash
./app 3 counter
./app 5 half
./app 5 "half l"
./app 3 dota
./app 10 war
./app 3 zelda
```

---

## Compilação

```bash
g++ -Wall -Wextra -Werror -Wshadow -Wpedantic -Wconversion -Wsign-conversion -Wnull-dereference -std=c++17 \
main.cpp Game.cpp Trie.cpp GamesDatabase.cpp -o app
```

Também é possível compilar com um comando mais simples:

```bash
g++ main.cpp Game.cpp Trie.cpp GamesDatabase.cpp -o app
```

---

## Execução

O programa deve ser executado pela linha de comando no formato:

```bash
./app k prefixo
```

Por exemplo:

```bash
./app 3 ha
```

Caso o prefixo possua espaços, ele deve ser passado entre aspas:

```bash
./app 3 "half l"
```

---

## Exemplo de Comportamento Esperado

Executando:

```bash
./app 3 counter
```

Uma saída possível é:

```text
Counter Strike Global Offensive | Counter Strike Global Offensive CS GO expands upon the team based action gameplay that it pioneered when it was launched 19 years ago CS GO features new maps characters weapons and game modes and delivers updated versions of the classic CS content de dust2 etc | 7428921
Counter Strike Source | Counter Strike Source blends Counter Strike s award winning teamplay action with the advanced technology of Source technology | 105942
Counter Strike Nexon Studio | Counter Strike Nexon Studio is a Free to Play MMOFPS offering competitive PvP and PvE action including content from the original Counter Strike and all new game modes map creation function weapons and hordes of Zombies | 41068
```

Executando:

```bash
./app 3 zelda
```

Caso não exista nenhum jogo com esse prefixo, a saída esperada é:

```text
No results found
```

---

## Observações sobre a Base de Jogos

A base de jogos é fornecida diretamente no código-fonte, nos arquivos `GamesDatabase.hpp` e `GamesDatabase.cpp`.

Como a base contém muitos jogos e muitas strings, a compilação pode demorar mais que o habitual.

Durante os testes locais, é possível reduzir temporariamente a quantidade de jogos no arquivo `GamesDatabase.cpp` para acelerar a compilação. Antes da entrega final, a base completa deve ser restaurada.

Também é possível compilar arquivos separadamente, usando arquivos objeto `.o`, para evitar recompilar toda a base a cada alteração.

Exemplo:

```bash
g++ -c GamesDatabase.cpp
g++ -c Game.cpp
g++ -c Trie.cpp
g++ -c main.cpp
g++ main.o Game.o Trie.o GamesDatabase.o -o app
```

Depois disso, se apenas `main.cpp` ou `Trie.cpp` forem alterados, basta recompilar os arquivos modificados e fazer a ligação novamente.

---

## Observações

Este projeto foi desenvolvido para fins acadêmicos como parte da disciplina de Estrutura de Dados.

A implementação tem como foco:

- compreensão da estrutura de dados Trie;
- prática de buscas por prefixo;
- normalização de strings;
- uso de ponteiros para associar dados à estrutura;
- implementação manual de ordenação;
- manutenção dos melhores `k` resultados durante a coleta;
- análise de complexidade algorítmica;
- organização modular de código em C++.
````