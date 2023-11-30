  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  #include <locale.h>

  const int MAX_ALUNOS = 2500;
  const int MAX_UCS = 200;
  const int MAX_EXAMES = 900;
  const int MAX_SALAS = 200;
  const int BUFFER_SIZE = 1056;

  // estrutura para armazenar informações de um aluno
  typedef struct
  {
    char *nome;
    char *regime;
    char *ano_matricula;
    char *numero_aluno;
    char *curso;
  } Aluno;

  // estrutura para armazenar informações de uma unidade curricular
  typedef struct
  {
    int id;
    char *nome;
    char *docente;
    char *ano_matricula;
    char *semestre;
  } UC;

  // estrutura para armazenar informações de um exame
  typedef struct
  {
    int id;
    UC uc;
    char *epoca;
    char *data;
    char *hora;
    int duracao;
    char *sala;
    int numSalas;
    Aluno alunos_inscritos[100];
    int num_alunos_inscritos;
    int id_sala;
  } Exame;

  // estrutura para armazenar informações de uma sala
  typedef struct
  {
    char designacao[10];
    int lotacao;
  } Sala;

  int ler_ficheiro_alunos(Aluno *alunos, int *num_alunos)
  {
    char buffer[BUFFER_SIZE];
    int linha_cont = 0; // contador de linhas
    FILE *f;
    // abre o arquivo em modo de leitura
    f = fopen("alunos.txt", "r");
    if (f == NULL)
    {
      perror("Erro ao abrir o arquivo");
      return 1;
    }
    // lê a primeira linha (cabeçalho) e ignora
    fgets(buffer, BUFFER_SIZE, f);

    // enquanto houverem linhas para serem lidas e o contador de linhas for menor ao máximo de alunos
    while (fgets(buffer, BUFFER_SIZE, f) != NULL && *num_alunos <= MAX_ALUNOS - 1)
    {
      // faz uma cópia da string para não alterar a original
      char linha[BUFFER_SIZE];
      strcpy(linha, buffer);

      // divide a string usando o tab como delimitador
      char *nome = strtok(linha, "\t");
      char *regime = strtok(NULL, "\t");
      char *ano_matricula = strtok(NULL, "\t");
      char *numero_aluno = strtok(NULL, "\t");
      char *curso = strtok(NULL, "\t");

      // Verifica se todos os campos foram lidos corretamente
      if (nome == NULL || regime == NULL || ano_matricula == NULL || numero_aluno == NULL || curso == NULL)
      {
        fprintf(stderr, "Erro ao ler os campos do ficheiro\n");
        return 1;
      }

      // Aloca memória dinamicamente para armazenar cada informação de aluno
      alunos[*num_alunos].nome = malloc(strlen(nome) + 1);
      alunos[*num_alunos].regime = malloc(strlen(regime) + 1);
      alunos[*num_alunos].ano_matricula = malloc(strlen(ano_matricula) + 1);
      alunos[*num_alunos].numero_aluno = malloc(strlen(numero_aluno) + 1);
      alunos[*num_alunos].curso = malloc(strlen(curso) + 1);

      // Verifica se a alocação de memória foi bem-sucedida
      if (alunos[*num_alunos].nome == NULL || alunos[*num_alunos].regime == NULL || alunos[*num_alunos].ano_matricula == NULL || alunos[*num_alunos].numero_aluno == NULL || alunos[*num_alunos].curso == NULL)
      {
        fprintf(stderr, "Erro ao alocar memória\n");
        return 1;
      }

      // copia as informações para a estrutura de aluno
      strcpy(alunos[*num_alunos].nome, nome);
      strcpy(alunos[*num_alunos].regime, regime);
      strcpy(alunos[*num_alunos].ano_matricula, ano_matricula);
      strcpy(alunos[*num_alunos].numero_aluno, numero_aluno);
      strcpy(alunos[*num_alunos].curso, curso);

      (*num_alunos)++; // incrementa o contador de alunos
      linha_cont++;    // incrementa o contador de linhas

      // Verifica se o número de linhas lidas é maior que o número máximo de alunos permitidos
      if (linha_cont > MAX_ALUNOS)
      {
        fprintf(stderr, "O número máximo de alunos permitidos foi ultrapassado\n");
        return 1;
      }

      // Verifica se o número de alunos lidos é maior que o número máximo de alunos permitidos
      if (*num_alunos > MAX_ALUNOS)
      {
        fprintf(stderr, "O número máximo de alunos permitidos foi ultrapassado\n");
        return 1;
      }
    }

    // fecha o arquivo
    fclose(f);
    return 0;
  }

  void exibir_alunos(Aluno *alunos, int num_alunos)
  {
    // Imprime o cabeçalho da tabela
    printf("Nome\t\t\tRegime\tAno Matrícula\tNúmero Aluno\tCurso\n");
    // Percorre todo o vetor de alunos
    for (int i = 0; i < num_alunos; i++)
    {
      // Imprime os dados de cada aluno
      printf("%s\t\t%s\t\t%s\t\t%s\t\t%s\n", alunos[i].nome, alunos[i].regime, alunos[i].ano_matricula, alunos[i].numero_aluno, alunos[i].curso);
    }
  }

  void salvar_alunos(Aluno *alunos, int num_alunos)
  {
    FILE *f = fopen("alunosguardado.txt", "w");
    if (f == NULL)
    {
      perror("Erro ao abrir o arquivo");
      return;
    }

    // escreve o cabeçalho no arquivo
    fprintf(f, "Nome\tRegime\tAno\tNumero\tCurso\n");

    // Percorre o vetor de alunos
    for (int i = 0; i < num_alunos; i++)
    {
      // escreve as informações do aluno no arquivo, separadas por tabulação
      fprintf(f, "%s\t%s\t%s\t%s\t%s\n", alunos[i].nome, alunos[i].regime, alunos[i].ano_matricula, alunos[i].numero_aluno, alunos[i].curso);
    }

    // fecha o arquivo
    fclose(f);
  }

  void criar_aluno(Aluno *alunos, int *num_alunos)
  {
    char nome_aluno[50];
    char regime_aluno[50];
    char ano_matricula_aluno[50];
    char numero_aluno[50];
    char curso_aluno[50];
    int ano_matricula;

    printf("Digite o nome do aluno:\n");
    fscanf(stdin, " %[^\n]", nome_aluno); //scanf não iria ler espaços e fgets ou outras formas não resultaram, sendo esta a única forma que foi encontrada para não dar erro e ler bem os dados
    printf("Digite o regime do aluno: \n");
    scanf("%s", regime_aluno);
    do
    {
      printf("Digite o ano de matrícula do aluno (1-3):\n ");
      scanf("%s", ano_matricula_aluno);
      ano_matricula = strtol(ano_matricula_aluno, NULL, 10);
    } while (ano_matricula < 1 || ano_matricula > 3);
    printf("Digite o número do aluno: ");
    scanf("%s", numero_aluno);
    for (int i = 0; i < *num_alunos; i++)
    {
      if (strcmp(numero_aluno, alunos[i].numero_aluno) == 0)
      {
        printf("O número do aluno já existe. Por favor, digite um número diferente.\n");
        return;
      }
    }
    printf("Digite o curso do aluno: ");
    scanf("%s", curso_aluno);

    // Aloca memória dinamicamente para armazenar cada informação de aluno
    alunos[*num_alunos].nome = malloc(strlen(nome_aluno) + 1);
    alunos[*num_alunos].regime = malloc(strlen(regime_aluno) + 1);
    alunos[*num_alunos].ano_matricula = malloc(strlen(ano_matricula_aluno) + 1);
    alunos[*num_alunos].numero_aluno = malloc(strlen(numero_aluno) + 1);
    alunos[*num_alunos].curso = malloc(strlen(curso_aluno) + 1);

    // Verifica se a memória foi alocada corretamente
    if (alunos[*num_alunos].nome == NULL || alunos[*num_alunos].regime == NULL || alunos[*num_alunos].ano_matricula == NULL || alunos[*num_alunos].numero_aluno == NULL || alunos[*num_alunos].curso == NULL)
    {
      perror("Erro ao alocar memória");
      return;
    }

    // Copia os dados do novo aluno para a estrutura
    strcpy(alunos[*num_alunos].nome, nome_aluno);
    strcpy(alunos[*num_alunos].regime, regime_aluno);
    strcpy(alunos[*num_alunos].ano_matricula, ano_matricula_aluno);
    strcpy(alunos[*num_alunos].numero_aluno, numero_aluno);
    strcpy(alunos[*num_alunos].curso, curso_aluno);

    // Incrementa o contador num_alunos para refletir a adição de um novo aluno
    (*num_alunos)++;
  }
  // Esta função pede ao utilizador que Insira os dados do novo aluno, lê os dados a partir do teclado e armazena-os na estrutura de dados correspondente. Por fim, incrementa o valor da variável num_alunos, para reflectir o facto de ter sido adicionado um novo aluno ao registo.

  void alterar_aluno(Aluno *alunos, int num_alunos)
  {
    char numero_aluno[50];
    int indice_aluno = -1;

    printf("Digite o número do aluno a ser alterado: ");
    scanf("%s", numero_aluno);
    // Procura o aluno pelo número de aluno
    for (int i = 0; i < num_alunos; i++)
    {
      if (strcmp(numero_aluno, alunos[i].numero_aluno) == 0)
      {
        indice_aluno = i;
        break;
      }
    }

    if (indice_aluno == -1)
    {
      printf("Aluno não encontrado.\n");
      return;
    }

    printf("Digite o novo nome do aluno:\n");
    fflush(stdin);
    fscanf(stdin, "%[^\n]", alunos[indice_aluno].nome); //mais uma vez o scanf não iria ler espaços e as outras formas de entrada de texto não estavam a funcionar

    printf("Digite o novo regime do aluno:\n");
    scanf("%s", alunos[indice_aluno].regime);

    printf("Digite o novo ano de matrícula do aluno:\n");
    scanf("%s", alunos[indice_aluno].ano_matricula);

    printf("Digite o novo curso do aluno:\n");
    scanf("%s", alunos[indice_aluno].curso);
  }

  void apagar_aluno(Aluno *alunos, int *num_alunos)
  {
    char numero_aluno[50];
    int indice_aluno = -1;

    printf("Digite o número do aluno a ser apagado: ");
    scanf("%s", numero_aluno);

    // Procura o aluno pelo número de aluno
    for (int i = 0; i < *num_alunos; i++)
    {
      if (strcmp(numero_aluno, alunos[i].numero_aluno) == 0)
      {
        indice_aluno = i;
        break;
      }
    }

    if (indice_aluno == -1)
    {
      printf("Aluno não encontrado.\n");
      return;
    }

    // Desaloca memória dinâmica alocada para armazenar as informações do aluno
    free(alunos[indice_aluno].nome);
    free(alunos[indice_aluno].regime);
    free(alunos[indice_aluno].ano_matricula);
    free(alunos[indice_aluno].numero_aluno);
    free(alunos[indice_aluno].curso);

    // Remove o aluno do vetor de alunos
    for (int i = indice_aluno; i < *num_alunos - 1; i++)
    {
      alunos[i] = alunos[i + 1];
    }

    // Decrementa o contador num_alunos para refletir a remoção de um aluno
    (*num_alunos)--;
  }

  int ler_ficheiro_ucs(UC *ucs, int *num_ucs)
  {
    char buffer[BUFFER_SIZE];
    int linha_cont = 0; // contador de linhas
    FILE *f;
    int id = 1;

    // abre o arquivo em modo de leitura
    f = fopen("ucs.txt", "r");
    if (f == NULL)
    {
      perror("Erro ao abrir o arquivo");
      return 1;
    }
    // lê a primeira linha (cabeçalho) e salta
    fgets(buffer, BUFFER_SIZE, f);

    while (fgets(buffer, BUFFER_SIZE, f) != NULL && *num_ucs <= MAX_UCS - 1)
    {
      // faz uma cópia da string para não alterar a original
      char linha[BUFFER_SIZE];
      strcpy(linha, buffer);

      // divide a string usando o tab como delimitador
      char *nome = strtok(linha, "\t");
      char *docente = strtok(NULL, "\t");
      char *ano_matricula = strtok(NULL, "\t");
      char *semestre = strtok(NULL, "\t");

      // aloca memória dinamicamente para armazenar cada informação da uc
      ucs[*num_ucs].nome = malloc(strlen(nome) + 1);
      ucs[*num_ucs].docente = malloc(strlen(docente) + 1);
      ucs[*num_ucs].ano_matricula = malloc(strlen(ano_matricula) + 1);
      ucs[*num_ucs].semestre = malloc(strlen(semestre) + 1);

      // verifica se a alocação de memória foi bem-sucedida
      if (ucs[*num_ucs].nome == NULL || ucs[*num_ucs].docente == NULL || ucs[*num_ucs].ano_matricula == NULL || ucs[*num_ucs].semestre == NULL)
      {
        fprintf(stderr, "Erro ao alocar memória\n");
        return -1;
      }
      ucs[*num_ucs].id = id;
      // Atribui o ID único à UC
      ucs[*num_ucs].id = id++;
      // copia as informações para a estrutura de uc
      strcpy(ucs[*num_ucs].nome, nome);
      strcpy(ucs[*num_ucs].docente, docente);
      strcpy(ucs[*num_ucs].ano_matricula, ano_matricula);
      strcpy(ucs[*num_ucs].semestre, semestre);

      (*num_ucs)++; // incrementa o contador de ucs
      linha_cont++; // incrementa o contador de linhas
    }

    // fecha o arquivo
    fclose(f);

    return 0;
  }

  void exibir_ucs(UC *ucs, int num_ucs)
  {
    // imprime o cabeçalho da tabela
    printf("ID\tNome\tDocente\tAno\tSemestre\n");
    printf("--\t----\t-------\t---\t---------\n");

    // percorre todas as UCs armazenadas
    for (int i = 0; i < num_ucs; i++)
    {
      // imprime os dados de cada UC
      printf("%d\t%s\t%s\t%s\t%s\n", ucs[i].id, ucs[i].nome, ucs[i].docente, ucs[i].ano_matricula, ucs[i].semestre);
    }
  }

  void salvar_ucs(UC *ucs, int *num_ucs)
  {
    FILE *f = fopen("ucsguardado.txt", "w");
    if (f == NULL)
    {
      perror("Erro ao abrir o arquivo");
      return;
    }

    // escreve o cabeçalho no arquivo
    fprintf(f, "Nome\tDocente\tAno\tSemestre\n");

    // itera sobre o vetor de alunos
    for (int i = 0; i < *num_ucs; i++)
    {
      // escreve as informações do aluno no arquivo, separadas por tabulação
      fprintf(f, "%s\t%s\t%s\t%s\n", ucs[i].nome, ucs[i].docente, ucs[i].ano_matricula, ucs[i].semestre);
    }

    // fecha o arquivo
    fclose(f);
  }

void criar_uc(UC *ucs, int *num_ucs)
{
  char nome_uc[50];
  char docente_uc[50];
  char ano_matricula_uc[50];
  char semestre_uc[50];

  printf("Digite o nome da UC:\n");
  fscanf(stdin, " %[^\n]", nome_uc);
  printf("Digite o docente da UC:\n");
  fscanf(stdin, " %[^\n]", docente_uc);
  printf("Digite o ano de matrícula da UC:\n");
  scanf("%s", ano_matricula_uc);
  printf("Digite o semestre da UC:\n");
  scanf("%s", semestre_uc);

  // Aloca memória dinamicamente para armazenar cada informação da UC
  ucs[*num_ucs].nome = malloc(strlen(nome_uc) + 1);
  ucs[*num_ucs].docente = malloc(strlen(docente_uc) + 1);
  ucs[*num_ucs].ano_matricula = malloc(strlen(ano_matricula_uc) + 1);
  ucs[*num_ucs].semestre = malloc(strlen(semestre_uc) + 1);

  // Verifica se a memória foi alocada corretamente
  if (ucs[*num_ucs].nome == NULL || ucs[*num_ucs].docente == NULL || ucs[*num_ucs].ano_matricula == NULL || ucs[*num_ucs].semestre == NULL)
  {
    perror("Erro ao alocar memória");
    return;
  }

  // Copia os dados da nova UC para a estrutura
  ucs[*num_ucs].id = *num_ucs + 1;
  strcpy(ucs[*num_ucs].nome, nome_uc);
  strcpy(ucs[*num_ucs].docente, docente_uc);
  strcpy(ucs[*num_ucs].ano_matricula, ano_matricula_uc);
  strcpy(ucs[*num_ucs].semestre, semestre_uc);

  // Incrementa o contador num_ucs para refletir a adição de uma nova UC
  (*num_ucs)++;
}

void alterar_uc(UC *ucs, int *num_ucs)
{
  int id;
  printf("Insira o ID da UC que deseja alterar: ");
  scanf("%d", &id);

  // percorre todas as UCs armazenadas
  for (int i = 0; i < *num_ucs; i++)
  {
    // verifica se a UC atual possui o ID especificado pelo usuário
    if (ucs[i].id == id)
    {
      printf("Insira o novo nome da UC:\n");
      fflush(stdin);
      fscanf(stdin, "%[^\n]", ucs[i].nome);
      printf("Insira o novo docente da UC:\n");
      fflush(stdin);
      fscanf(stdin, "%[^\n]", ucs[i].docente);
      printf("Insira o novo ano de matrícula da UC:\n");
      scanf("%s", ucs[i].ano_matricula);
      printf("Insira o novo semestre da UC:\n");
      scanf("%s", ucs[i].semestre);
      return; // sai da função após alterar a UC
    }
  }

  // se nenhuma UC foi encontrada com o ID especificado, imprime uma mensagem de erro
  printf("Nenhuma UC com o ID %d foi encontrada.\n", id);
}

void apagar_uc(UC *ucs, int *num_ucs)
{
  int id;
  printf("Insira o ID da UC que deseja apagar: ");
  scanf("%d", &id);

  // percorre todas as UCs armazenadas
  for (int i = 0; i < *num_ucs; i++)
  {
    // verifica se a UC atual possui o ID especificado pelo usuário
    if (ucs[i].id == id)
    {
      // libera a memória alocada para armazenar os dados da UC
      free(ucs[i].nome);
      free(ucs[i].docente);
      free(ucs[i].ano_matricula);
      free(ucs[i].semestre);

      // remove a UC do vetor
      for (int j = i; j < *num_ucs - 1; j++)
      {
        ucs[j] = ucs[j + 1];
      }
      (*num_ucs)--; // atualiza o número de UCs

      for (int i = id - 1; i < *num_ucs; i++)
      {
        ucs[i].id--;
      }
      return; // sai da função após apagar a UC
    }
  }

  // se nenhuma UC foi encontrada com o ID especificado, imprime uma mensagem de erro
  printf("Nenhuma UC com o ID %d foi encontrada.\n", id);
}

int ler_ficheiro_exames(Exame *exames, int *num_exames)
{
  FILE *f;
  int id = 1;
  f = fopen("exames.txt", "r");
  if (f == NULL)
  {
    perror("Erro ao abrir o arquivo");
    return 1;
  }
  char nome_uc[BUFFER_SIZE], epoca[BUFFER_SIZE], data[BUFFER_SIZE], hora[BUFFER_SIZE], sala[BUFFER_SIZE];
  int duracao;
  // lê e descarta a primeira linha do ficheiro (cabeçalho)
  fscanf(f, "%*[^\n]\n");
  while (fscanf(f, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d\t%[^\n]\n", nome_uc, epoca, data, hora, &duracao, sala) == 6 && *num_exames <= MAX_EXAMES - 1)
  {
    // cria um novo exame e adiciona à lista de exames
    Exame novo_exame;
    novo_exame.id = *num_exames + 1;      // define o id do exame
    novo_exame.uc.nome = strdup(nome_uc); // copia o nome da UC para a estrutura
    novo_exame.epoca = strdup(epoca);     // copia a época para a estrutura
    novo_exame.data = strdup(data);       // copia a data para a estrutura
    novo_exame.hora = strdup(hora);       // copia a hora para a estrutura
    novo_exame.duracao = duracao;         // define a duração do exame
    novo_exame.sala = strdup(sala);       // copia a sala para a estrutura
    exames[*num_exames] = novo_exame;     // adiciona o exame à lista
    (*num_exames)++;                      // incrementa o contador de exames
  }
  fclose(f);
  return 0;
}

void exibir_exames(Exame *exames, int num_exames)
{
  // imprime o cabeçalho da tabela
  printf("ID\tUC\tEpoca\tData\tHora\tDuração\tSala\n");
  printf("--\t--\t-----\t----\t----\t--------\t----\n");

  // percorre todos os exames armazenados
  for (int i = 0; i < num_exames; i++)
  {
    // imprime os dados de cada exame
    printf("%d\t%s\t%s\t%s\t%s\t%d\t%s\n", exames[i].id, exames[i].uc.nome, exames[i].epoca, exames[i].data, exames[i].hora, exames[i].duracao, exames[i].sala);
  }
}

void salvar_exames(Exame *exames, int *num_exames)
{
  // abre o arquivo examesguardado.txt em modo de escrita
  FILE *f = fopen("examesguardado.txt", "w");
  if (f == NULL)
  {
    perror("Erro ao abrir o arquivo");
    return;
  }

  // escreve o cabeçalho no arquivo
  fprintf(f, "UC\tEpoca\tData\tHora\tDuração\tSala(s)\n");

  // itera sobre o vetor de exames
  for (int i = 0; i < *num_exames; i++)
  {
    // escreve as informações do exame no arquivo, separadas por tabulação
    fprintf(f, "%s\t%s\t%s\t%s\t%d\t%s\n", exames[i].uc.nome, exames[i].epoca, exames[i].data, exames[i].hora, exames[i].duracao, exames[i].sala);
  }
  // fecha o arquivo
  fclose(f);
}

void criar_exame(Exame *exames, int *num_exames, UC *ucs, int num_ucs)
{
  int id_uc;
  char epoca[50];
  char data[50];
  char hora[50];
  int duracao;
  char sala[50];
  int numSalas;

  printf("Digite o ID da UC do exame:\n");
  scanf("%d", &id_uc);
  printf("Digite a época do exame:\n");
  scanf("%s", epoca);
  printf("Digite a data do exame:\n");
  scanf("%s", data);
  printf("Digite a hora do exame:\n");
  scanf("%s", hora);
  printf("Digite a duração do exame (em minutos):\n");
  scanf("%d", &duracao);
  printf("Digite a sala do exame: \n");
  scanf("%s", sala);

  // Verifica se a UC existe
  UC *uc = NULL;
  for (int i = 0; i < num_ucs; i++)
  {
    if (ucs[i].id == id_uc)
    {
      uc = &ucs[i];
      break;
    }
  }
  if (uc == NULL)
  {
    printf("A UC com o ID %d não existe.\n", id_uc);
    return;
  }

  // Aloca memória dinamicamente para armazenar cada informação do exame
  exames[*num_exames].uc = *uc;
  exames[*num_exames].epoca = malloc(strlen(epoca) + 1);
  exames[*num_exames].data = malloc(strlen(data) + 1);
  exames[*num_exames].hora = malloc(strlen(hora) + 1);
  exames[*num_exames].sala = malloc(strlen(sala) + 1);

  // Verifica se a memória foi alocada corretamente
  if (exames[*num_exames].epoca == NULL || exames[*num_exames].data == NULL || exames[*num_exames].hora == NULL || exames[*num_exames].sala == NULL)
  {
    perror("Erro ao alocar memória");
    return;
  }

  // Copia os dados do novo exame para a estrutura
  exames[*num_exames].id = *num_exames + 1;
  strcpy(exames[*num_exames].epoca, epoca);
  strcpy(exames[*num_exames].data, data);
  strcpy(exames[*num_exames].hora, hora);
  exames[*num_exames].duracao = duracao;
  strcpy(exames[*num_exames].sala, sala);
  exames[*num_exames].numSalas = numSalas;
  exames[*num_exames].num_alunos_inscritos = 0;

  // Incrementa o contador num_exames para refletir a adição de um novo exame
  (*num_exames)++;
}

void alterar_exame(Exame *exames, int num_exames)
{
  // Pede o ID do exame a ser alterado
  int id;
  printf("Insira o ID do exame a ser alterado: ");
  scanf("%d", &id);

  // Percorre a lista de exames até encontrar o exame com o ID especificado
  for (int i = 0; i < num_exames; i++)
  {
    if (exames[i].id == id)
    {
      // Pega os novos dados do exame
      printf("Insira o novo nome da UC: ");
      scanf("%s", exames[i].uc.nome);
      printf("Insira a nova época do exame: ");
      scanf("%s", exames[i].epoca);
      printf("Insira a nova data do exame (formato dd/mm/aa): ");
      scanf("%s", exames[i].data);
      printf("Insira a nova hora do exame (formato hh:mm): ");
      scanf("%s", exames[i].hora);
      printf("Insira a nova duração do exame (em minutos): ");
      scanf("%d", &exames[i].duracao);
      printf("Insira a nova sala do exame: ");
      scanf("%s", exames[i].sala);
      break; // sai do loop assim que o exame é encontrado
    }
  }
}

void apagar_exame(Exame *exames, int *num_exames)
{
  int id;
  printf("Digite o ID do exame a ser apagado:\n");
  scanf("%d", &id);
  // procura o exame com o ID especificado
  int indice = -1;
  for (int i = 0; i < *num_exames; i++)
  {
    if (exames[i].id == id)
    {
      indice = i;
      break;
    }
  }

  // se o exame não foi encontrado, imprime uma mensagem de erro
  if (indice == -1)
  {
    printf("Exame com o ID especificado não encontrado.\n");
    return;
  }

  // move todos os elementos do vetor de exames para trás, sobrepondo o elemento a ser apagado
  for (int i = indice; i < *num_exames - 1; i++)
  {
    exames[i] = exames[i + 1];
  }

  for (int i = id - 1; i < *num_exames; i++)
  {
    exames[i].id--;
  }

  // decrementa o contador de exames
  (*num_exames)--;
}

int ler_ficheiro_salas(Sala *salas, int *num_salas)
{
  char buffer[BUFFER_SIZE];
  FILE *f;
  f = fopen("salas.txt", "r");
  if (f == NULL)
  {
    perror("Error opening the file");
    return 1;
  }
  fgets(buffer, BUFFER_SIZE, f);
  while (fgets(buffer, BUFFER_SIZE, f) != NULL && *num_salas <= MAX_SALAS - 1)
  {
    char linha[BUFFER_SIZE];
    strcpy(linha, buffer);
    char *designacao = strtok(linha, "\t");
    char *lotacao = strtok(NULL, "\t");

    if (designacao == NULL || lotacao == NULL)
    {
      fprintf(stderr, "Erro ao ler o arquivo\n");
      return 1;
    }
    strcpy(salas[*num_salas].designacao, designacao);
    salas[*num_salas].lotacao = atoi(lotacao);

    (*num_salas)++;
  }

  fclose(f);

  return 0;
}

int criar_sala(Sala *salas, int *num_salas)
{
  // verifica se o número de salas atingiu o máximo permitido
  if (*num_salas == MAX_SALAS)
  {
    fprintf(stderr, "Número máximo de salas já foi atingido\n");
    return 1;
  }

  // pede ao utilizador para inserir a informação da sala
  printf("Insira a designação da sala: ");
  scanf("%s", salas[*num_salas].designacao);
  printf("Insira a lotação da sala: ");
  scanf("%d", &salas[*num_salas].lotacao);

  (*num_salas)++; // atualiza o número de salas

  return 0;
}

int apagar_sala(Sala *salas, int *num_salas)
{
  // lê a designação da sala a ser apagada
  char designacao[10];
  printf("Insira a designação da sala a ser apagada: ");
  scanf("%s", designacao);

  // procura a sala na lista de salas
  int i;
  for (i = 0; i < *num_salas; i++)
  {
    if (strcmp(salas[i].designacao, designacao) == 0)
    {
      break;
    }
  }

  // verifica se a sala foi encontrada
  if (i == *num_salas)
  {
    printf("Sala não encontrada.\n");
    return 1;
  }

  // remove a sala da lista de salas
  (*num_salas)--;
  for (; i < *num_salas; i++)
  {
    salas[i] = salas[i + 1];
  }

  printf("Sala apagada com sucesso.\n");

  return 0;
}

void alterar_sala(Sala *salas, int num_salas)
{
  // lê a designação da sala a alterar
  char designacao[10];
  printf("Insira a designação da sala a alterar: ");
  scanf("%s", designacao);

  // procura a sala pelo nome
  Sala *sala = NULL;
  for (int i = 0; i < num_salas; i++)
  {
    if (strcmp(salas[i].designacao, designacao) == 0)
    {
      sala = &salas[i];
      break;
    }
  }

  // verifica se a sala foi encontrada
  if (sala == NULL)
  {
    printf("Sala não encontrada\n");
    return;
  }

  // lê a nova lotação da sala
  int nova_lotacao;
  printf("Insira a nova lotação da sala: ");
  scanf("%d", &nova_lotacao);

  // altera a lotação da sala
  sala->lotacao = nova_lotacao;

  printf("Sala alterada com sucesso\n");
}

void exibir_salas(Sala *salas, int num_salas)
{
  printf("Designação\tLotação\n");
  for (int i = 0; i < num_salas; i++)
  {
    printf("%s\t%d\n", salas[i].designacao, salas[i].lotacao);
  }
}

void salvar_salas(Sala *salas, int *num_salas)
{
  // abre o arquivo em modo de escrita
  FILE *f = fopen("salasguardado.txt", "w");
  if (f == NULL)
  {
    perror("Erro ao abrir o arquivo");
    return;
  }

  // escreve o cabeçalho no arquivo
  fprintf(f, "Sala\tLotação\n");

  // itera sobre o vetor de alunos
  for (int i = 0; i < *num_salas; i++)
  {
    // escreve as informações do aluno no arquivo, separadas por tabulação
    fprintf(f, "%s\t%d\n", salas[i].designacao, salas[i].lotacao);
  }

  // fecha o arquivo
  fclose(f);
}

void inscrever_aluno_exame(Exame *exames, int num_exames, Aluno *alunos, int num_alunos, int id_exame, char *numero_aluno)
{
  // procura o exame pelo id
  int indice_exame = -1;
  printf("Insira o ID do exame pretendido:\n");
  scanf("%d", &id_exame);
  for (int i = 0; i < num_exames; i++)
  {
    if (exames[i].id == id_exame)
    {
      indice_exame = i;
      break;
    }
  }
  // se o exame não foi encontrado, imprime uma mensagem de erro e retorna
  if (indice_exame == -1)
  {
    printf("Exame com id %d não encontrado\n", id_exame);
    return;
  }
  // procura o aluno pelo número
  printf("Digite o número do aluno a ser inscrito: ");
  scanf("%s", numero_aluno);
  int indice_aluno = -1;
  for (int i = 0; i < num_alunos; i++)
  {
    if (strcmp(alunos[i].numero_aluno, numero_aluno) == 0)
    {
      indice_aluno = i;
      break;
    }
  }
  // se o aluno não foi encontrado, imprime uma mensagem de erro e retorna
  if (indice_aluno == -1)
  {
    printf("Aluno com número %s não encontrado\n", numero_aluno);
    return;
  } // Verifica se a época do exame é especial
  if (strcmp(exames[indice_exame].epoca, "especial") == 0)
  {
    // Verifica se o aluno tem permissão para se inscrever em exames especiais
    if (strcmp(alunos[indice_aluno].regime, "atleta") == 0 || strcmp(alunos[indice_aluno].regime, "trabalhador-estudante") == 0 || strcmp(alunos[indice_aluno].regime, "dirigente-associativo") == 0 || (strcmp(alunos[indice_aluno].ano_matricula, "3") == 0))
    {
      // Inscreve o aluno no exame
      exames[indice_exame].alunos_inscritos[exames[indice_exame].num_alunos_inscritos] = alunos[indice_aluno];
      exames[indice_exame].num_alunos_inscritos++;
      printf("Aluno inscrito com sucesso no exame com id %d\n", id_exame);
    }
    else
    {
      printf("O aluno não tem permissão para se inscrever em exames especiais.\n");
    }
  }
  else
  {
    // Inscreve o aluno no exame
    exames[indice_exame].alunos_inscritos[exames[indice_exame].num_alunos_inscritos] = alunos[indice_aluno];
    exames[indice_exame].num_alunos_inscritos++;
    printf("Aluno inscrito com sucesso no exame com id %d\n", id_exame);
  }
}

void ver_exame(Exame *exames, int num_exames)
{
  int id_exame;
  printf("Insira o id do exame que deseja ver:\n");
  scanf("%d", &id_exame);

  // procura o exame pelo id
  Exame exame;
  int exame_encontrado = 0;
  for (int i = 0; i < num_exames; i++)
  {
    if (exames[i].id == id_exame)
    {
      exame = exames[i];
      exame_encontrado = 1;
      break;
    }
  }

  // verifica se o exame foi encontrado
  if (!exame_encontrado)
  {
    fprintf(stderr, "Exame com o id %d não encontrado\n", id_exame);
    return;
  }

  // imprime os dados do exame
  printf("Id: %d\n", exame.id);
  printf("Nome: %s\n", exame.uc.nome);
  printf("Epoca: %s\n", exame.epoca);
  printf("Data: %s\n", exame.data);
  printf("Hora: %s\n", exame.hora);
  printf("Sala: %s\n", exame.sala);

  // imprime o nome dos alunos inscritos
  printf("Alunos inscritos: ");
  for (int i = 0; i < exame.num_alunos_inscritos; i++)
  {
    printf("%s", exame.alunos_inscritos[i].nome);
    if (i != exame.num_alunos_inscritos - 1)
    {
      printf(", ");
    }
  }
  printf("\n");
}
void cancelar_inscricao_exame(Exame *exames, int num_exames, int id_exame, char *numero_aluno)
{
  // procura o exame pelo id
  int indice_exame = -1;
  printf("Insira o ID do exame pretendido:\n");
  scanf("%d", &id_exame);
  for (int i = 0; i < num_exames; i++)
  {
    if (exames[i].id == id_exame)
    {
      indice_exame = i;
      break;
    }
  }
  // se o exame não foi encontrado, imprime uma mensagem de erro e retorna
  if (indice_exame == -1)
  {
    printf("Exame com id %d não encontrado\n", id_exame);
    return;
  }

  // procura o aluno pelo número
  printf("Digite o número do aluno a ser removido: ");
  scanf("%s", numero_aluno);
  int indice_aluno = -1;
  for (int i = 0; i < exames[indice_exame].num_alunos_inscritos; i++)
  {
    if (strcmp(exames[indice_exame].alunos_inscritos[i].numero_aluno, numero_aluno) == 0)
    {
      indice_aluno = i;
      break;
    }
  }
  // se o aluno não foi encontrado, imprime uma mensagem de erro e retorna
  if (indice_aluno == -1)
  {
    printf("Aluno com número %s não encontrado no exame com id %d\n", numero_aluno, id_exame);
    return;
  }

  // remove o aluno do exame
  for (int i = indice_aluno; i < exames[indice_exame].num_alunos_inscritos - 1; i++)
  {
    exames[indice_exame].alunos_inscritos[i] = exames[indice_exame].alunos_inscritos[i + 1];
  }
  exames[indice_exame].num_alunos_inscritos--;
  printf("Inscrição cancelada com sucesso no exame com id %d\n", id_exame);
}

void mostrar_inscricoes_aluno(Exame *exames, int num_exames)
{
  char numero_aluno[MAX_ALUNOS];

  // Pede ao utilizador o número do aluno
  printf("Insira o número do aluno: ");
  scanf("%s", numero_aluno);

  // Inicializa a variável que indica se o aluno foi encontrado
  int encontrado = 0;

  // Percorre todos os exames
  for (int i = 0; i < num_exames; i++)
  {
    // Percorre todos os alunos inscritos no exame
    for (int j = 0; j < exames[i].num_alunos_inscritos; j++)
    {
      // Verifica se o número do aluno inserido pelo utilizador corresponde ao número de algum aluno inscrito no exame
      if (strcmp(exames[i].alunos_inscritos[j].numero_aluno, numero_aluno) == 0)
      {
        // Se sim, imprime o id do exame e o nome da UC a que pertence
        printf("Exame com id %d, UC %s\n", exames[i].id, exames[i].uc.nome);
        encontrado = 1;
      }
    }
  }

  // Se o aluno não foi encontrado em nenhum exame, imprime uma mensagem de erro
  if (encontrado == 0)
  {
    printf("O aluno com número %s não está inscrito em nenhum exame.\n", numero_aluno);
  }
}



int main()
{
  setlocale(LC_ALL, "Portuguese");
  Aluno alunos[2500];
  int num_alunos = 0;
  UC ucs[200];
  int num_ucs = 0;
  Sala salas[MAX_SALAS];
  int num_salas = 0;
  Exame exames[MAX_EXAMES];
  int num_exames = 0;
  char buffer[2500];
  int linha_cont = 0; // contador de linhas
  int id_exame;
  char numero_aluno[20];
  FILE *f;

  for (int i = 0; i < MAX_EXAMES; i++)
  {
    exames[i].numSalas = 0;
    exames[i].num_alunos_inscritos = 0;
  }

  ler_ficheiro_alunos(alunos, &num_alunos);
  ler_ficheiro_ucs(ucs, &num_ucs);
  ler_ficheiro_salas(salas, &num_salas);
  ler_ficheiro_exames(exames, &num_exames);

  int opcao;
  do
  {
    // mostra o menu de opções
    printf("\nMenu de opções:\n");
    printf("1 - Visualizar, criar, alterar e apagar dados dos alunos\n");
    printf("2 - Visualizar, criar, alterar e apagar dados das unidades curriculares\n");
    printf("3 - Visualizar, criar, alterar e apagar exames\n");
    printf("4 - Visualizar, criar, alterar e apagar salas\n");
    printf("5 - Gerar ficheiros para os dados\n");
    printf("6 - Inscrever ou cancelar inscrição num exame\n");
    printf("7 - Visualizar inscrições num exame\n");
    printf("0 - Sair\n");
    printf("Insira a opção desejada: ");
    scanf("%d", &opcao);

    // verifica qual opção foi escolhida
    switch (opcao)
    {
    case 1:
      // cria, altera ou apaga dados dos alunos
      printf("\nOpção 1 - Visualizar, criar, alterar e apagar dados dos alunos:\n");
      printf("1 - Criar novo aluno\n");
      printf("2 - Visualizar dados dos alunos\n");
      printf("3 - Alterar dados de um aluno\n");
      printf("4 - Apagar aluno\n");
      printf("Insira a subopção desejada: ");
      int subopcao;
      scanf("%d", &subopcao);
      switch (subopcao)
      {
      case 1:
        criar_aluno(alunos, &num_alunos);
        break;
      case 2:
        exibir_alunos(alunos, num_alunos);
        break;
      case 3:
        alterar_aluno(alunos, num_alunos);
        break;
      case 4:
        apagar_aluno(alunos, &num_alunos);
        break;
      default:
        printf("Subopção inválida.\n");
        break;
      }
      break;
    case 2:
      // cria, altera ou apaga dados das unidades curriculares
      printf("\nOpção 2 - Criar, alterar e apagar dados das unidades curriculares:\n");
      printf("1 - Criar nova unidade curricular\n");
      printf("2 - Visualizar UCs existentes\n");
      printf("3 - Alterar dados de uma unidade curricular\n");
      printf("4 - Apagar unidade curricular\n");
      printf("Insira a subopção desejada: ");
      scanf("%d", &subopcao);
      switch (subopcao)
      {
      case 1:
        criar_uc(ucs, &num_ucs);
        break;
      case 2:
        exibir_ucs(ucs, num_ucs);
        break;
      case 3:
        alterar_uc(ucs, &num_ucs);
        break;
      case 4:
        apagar_uc(ucs, &num_ucs);
        break;
      default:
        printf("Subopção inválida.\n");
        break;
      }
      break;
    case 3:
      // cria e configura exames
      printf("\nOpção 3 - Visualizar, criar, alterar e apagar exames:\n");
      printf("1 - Criar novo exame\n");
      printf("2 - Visualizar exames\n");
      printf("3 - Alterar dados de um exame\n");
      printf("4 - Apagar exame\n");
      scanf("%d", &subopcao);
      switch (subopcao)
      {
      case 1:
        criar_exame(exames, &num_exames, ucs, num_ucs);
        break;
      case 2:
        exibir_exames(exames, num_exames);
        break;
      case 3:
        alterar_exame(exames, num_exames);
        break;
      case 4:
        apagar_exame(exames, &num_exames);
        break;
      default:
        printf("Subopção inválida.\n");
        break;
      }
      break;
    case 4:
      // cria e configura exames
      printf("\nOpção 4 - Visualizar, criar, alterar e apagar salas\n");
      printf("1 - Criar nova sala\n");
      printf("2 - Visualizar salas\n");
      printf("3 - Alterar dados de uma sala\n");
      printf("4 - Apagar sala\n");

      scanf("%d", &subopcao);
      switch (subopcao)
      {
      case 1:
        criar_sala(salas, &num_salas);
        break;
      case 2:
        exibir_salas(salas, num_salas);
        break;
      case 3:
        alterar_sala(salas, num_salas);
        break;
      case 4:
        apagar_sala(salas, &num_salas);
        break;
      default:
        printf("Subopção inválida.\n");
        break;
      }
      break;

    case 5:
      printf("\nOpção 5 - Guardar dados armazenados na memória\n");
      printf("1 - Guardar alunos\n");
      printf("2 - Guardar unidades curriculares\n");
      printf("3 - Guardar exames\n");
      printf("4 - Guardar salas\n");
      scanf("%d", &subopcao);
      switch (subopcao)
      {
      case 1:
        salvar_alunos(alunos, num_alunos);
        break;
      case 2:
        salvar_ucs(ucs, &num_ucs);
        break;
      case 3:
        salvar_exames(exames, &num_exames);
        break;
      case 4:
        salvar_salas(salas, &num_salas);
        break;
      default:
        printf("Subopção inválida.\n");
        break;
      }
      break;

    case 6:
      // cria e configura inscrições
      printf("\nOpção 6 - Inscrever e cancelar inscrição\n");
      printf("1 - Inscrever aluno\n");
      printf("2 - Cancelar inscrição\n");
      scanf("%d", &subopcao);
      switch (subopcao)
      {
      case 1:
        inscrever_aluno_exame(exames, num_exames, alunos, num_alunos, id_exame, numero_aluno);
        break;
      case 2:
        cancelar_inscricao_exame(exames, num_exames, id_exame, numero_aluno);
        break;
      default:
        printf("Subopção inválida.\n");
        break;
      }
      break;
    case 7:
      printf("Opção 7 - Visualizar inscrições nos exames\n");
      printf("1 - Ver inscrições do exame\n");
      printf("2 - Ver inscrições do aluno\n");
      scanf("%d", &subopcao);
      switch (subopcao)
      {
      case 1:
        ver_exame(exames, num_exames);
        break;
      case 2:
        mostrar_inscricoes_aluno(exames, num_exames);
        break;
      default:
        printf("Subopção inválida.\n");
        break;
      }
      break;
    case 0:
      printf("A sair do programa...\n");
      break;
    }

  } while (opcao != 0);

  return 0;
}
