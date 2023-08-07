## CSV Parser

```c++
#include <iostream>
#include "../parser.hpp"

using namespace aria::csv;

int main() {
  std::ifstream f("some_file.csv");
  CsvParser parser(f);

  for (auto& row : parser) {
    for (auto& field : row) {
      std::cout << field << " | ";
    }
    std::cout << std::endl;
  }
}
```

## Datasets

- `players.csv` -> `sofifa_id,name,player_positions`
- `rating.csv` -> `user_id,sofifa_id,rating`
- `tags.csv` -> `user_id,sofifa_id,tag`
- `minirating.csv` -> Dataset de `rating` menor para testes

## Pesquisas

### Nomes de jogadores

Retorna informações de todos os jogadores contento um dado prefixo no nome.

```
$ player <name|prefix>
```

- Implementar árvore trie com os nomes dos jogadores, guardando o valor hash do player nas folhas.
- Implementar hashmap para busca com o `sofifa_id`, armazenando `name`, `player_positions`, soma das avaliações (para calcular a média) e número de avaliações.

### Usuários

Retorna 20 jogadores mais bem avaliados pelo usuário (em ordem decrescente), com nota, média global do jogador e contagem de avaliações.

```
$ user <userID>
```

- Implementar um hashmap com a lista de avaliações de cada usuário (pode ordenar as listas na a criação da estrutura ou durante a busca)

### Ranking em uma determinada posição

Dado um número `n` e uma posição, retornar os `n` jogadores mais bem avaliados da posição com no mínimo 1000 avaliações.

```
$ top<n> <position>
```

- Implementar uma lista de `sofifa_id`'s ordenada decrescentemente por `rating` para jogadores com mais de 1000 avaliações. Retornar os `n` primeiros jogadores com a dada posição.

### Pesquisa sobre tags

Dado uma lista de tags, retornar uma lista com os jogadores que apresentam todas as tags.

```
$ tags <list of tags>
```

- Implementar trie ou hashmap com cada tag contendo uma lista de jogadores que a contém.