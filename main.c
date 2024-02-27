//by: Jo�o Ifran e Cau� Ribeiro
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 50
#define CPF_LENGTH 11

typedef struct {
  char cpf[CPF_LENGTH + 1];
  char nome[MAX_STRING_LENGTH];
  char sobrenome[MAX_STRING_LENGTH];
  char email[MAX_STRING_LENGTH];
  int telefone;
} hospede;

void menu();
bool cpfValido(const char *cpf);
bool cpfExiste(const char *cpfProcurado);
void cadastrar();
void removerCadastro();
void buscarPorCadastro();
void atualizarCadastro();
void imprimirTodosCadastrados();
void gerarRelatorio();

int main() {
  setlocale(LC_ALL, "Portuguese");
  menu();
  return 0;
}

void menu() {
  int opcao;
  hospede novoHospede;
  FILE *arquivo;

  do {
    printf("// ----- // ----- // CADASTRO H�SPEDE // ----- // ----- //\n");
    printf("[1] Buscar por cadastro \n[2] - Cadastrar novo\n[3] - Atualizar "
           "cadastro\n[4] - Remover cadastro\n[5] - Imprimir todos "
           "cadastrados\n[6] - Gerar relatorio\n[7] - Finalizar\n");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      buscarPorCadastro();
      break;
    case 2:
      cadastrar();
      break;
    case 3:
      atualizarCadastro();
      break;
    case 4:
      removerCadastro();
      break;
    case 5:
      imprimirTodosCadastrados();
      break;
    case 6:
      gerarRelatorio();
      break;
    case 7:
      printf("Programa finalizado.\n");
      break;
    default:
      printf("Opcao invalida!\n");
      break;
    }
  } while (opcao != 7);
}

bool cpfValido(const char *cpf) {
  return strlen(cpf) == CPF_LENGTH;
}

bool cpfExiste(const char *cpfProcurado) {
  FILE *arquivo;
  hospede hospedeAtual;

  arquivo = fopen("hospedes.dat", "rb");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de hospedes!\n");
    return false;
  }

  while (fread(&hospedeAtual, sizeof(hospede), 1, arquivo)) {
    if (strcmp(hospedeAtual.cpf, cpfProcurado) == 0) {
      fclose(arquivo);
      return true;
    }
  }

  fclose(arquivo);
  return false;
}

void cadastrar() {
  hospede novoHospede;
  printf("Digite o cpf do hospede para o cadastro: \n");
  scanf("%11s", novoHospede.cpf);

  if (!cpfValido(novoHospede.cpf)) {
    printf("CPF invalido! O CPF deve ter 11 d�gitos.\n");
    return;
  }

  if (cpfExiste(novoHospede.cpf)) {
    printf("CPF j� cadastrado! Nao e possovel cadastrar novamente.\n");
    return;
  }

  printf("Digite o nome do hospede para o cadastro: \n");
  scanf("%49s", novoHospede.nome);
  printf("Digite o sobrenome do hospede para o cadastro: \n");
  scanf("%49s", novoHospede.sobrenome);
  printf("Digite o email do hospede para o cadastro: \n");
  scanf("%49s", novoHospede.email);
  printf("Digite o telefone do hospede para o cadastro: \n");
  scanf("%d", &novoHospede.telefone);

  FILE *arquivo = fopen("hospedes.dat", "ab");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de hospedes para escrita!\n");
    return;
  }

  fwrite(&novoHospede, sizeof(hospede), 1, arquivo);
  fclose(arquivo);

  printf("Cadastro realizado com sucesso!\n");
}

void buscarPorCadastro() {
  char cpfProcurado[CPF_LENGTH + 1];
  hospede hospedeEncontrado;
  FILE *arquivo = fopen("hospedes.dat", "rb");

  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de hospedes para leitura!\n");
    return;
  }

  printf("Digite o CPF do hospede que deseja buscar: ");
  scanf("%s", cpfProcurado);

  while (fread(&hospedeEncontrado, sizeof(hospede), 1, arquivo)) {
    if (strcmp(hospedeEncontrado.cpf, cpfProcurado) == 0) {
      printf("Hospede encontrado:\n");
      printf("CPF: %s\n", hospedeEncontrado.cpf);
      printf("Nome: %s %s\n", hospedeEncontrado.nome,
             hospedeEncontrado.sobrenome);
      printf("Email: %s\n", hospedeEncontrado.email);
      printf("Telefone: %d\n", hospedeEncontrado.telefone);
      fclose(arquivo);
      return;
    }
  }

  printf("Hospede com o CPF %s nao encontrado.\n", cpfProcurado);
  fclose(arquivo);
}

void atualizarCadastro() {
  char cpfProcurado[CPF_LENGTH + 1];
  hospede hospedeAtualizado;
  FILE *arquivo = fopen("hospedes.dat", "r+b");

  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de hospedes para atualizacao!\n");
    return;
  }

  printf("Digite o CPF do hospede que deseja atualizar: ");
  scanf("%s", cpfProcurado);

  while (fread(&hospedeAtualizado, sizeof(hospede), 1, arquivo)) {
    if (strcmp(hospedeAtualizado.cpf, cpfProcurado) == 0) {
      printf("Digite o nome atualizado do hospede: ");
      scanf("%s", hospedeAtualizado.nome);
      printf("Digite o sobrenome atualizado do hospede: ");
      scanf("%s", hospedeAtualizado.sobrenome);
      printf("Digite o email atualizado do hospede: ");
      scanf("%s", hospedeAtualizado.email);
      printf("Digite o telefone atualizado do hospede: ");
      scanf("%d", &hospedeAtualizado.telefone);

      fseek(arquivo, -sizeof(hospede), SEEK_CUR);
      fwrite(&hospedeAtualizado, sizeof(hospede), 1, arquivo);
      fclose(arquivo);
      printf("Cadastro atualizado com sucesso!\n");
      return;
    }
  }

  printf("Hospede com o CPF %s n�o encontrado.\n", cpfProcurado);
  fclose(arquivo);
}

void removerCadastro() {
  char cpfProcurado[CPF_LENGTH + 1];
  hospede hospedeRemovido;
  FILE *arquivo = fopen("hospedes.dat", "r+b");

  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de hospedes para remo��o!\n");
    return;
  }

  printf("Digite o CPF do hospede que deseja remover: ");
  scanf("%11s", cpfProcurado);

  while (fread(&hospedeRemovido, sizeof(hospede), 1, arquivo)) {
    if (strcmp(hospedeRemovido.cpf, cpfProcurado) == 0) {
      memset(hospedeRemovido.cpf, '\0', sizeof(hospedeRemovido.cpf));

      fseek(arquivo, -sizeof(hospede), SEEK_CUR);
      fwrite(&hospedeRemovido, sizeof(hospede), 1, arquivo);
      fclose(arquivo);
      printf("Cadastro removido com sucesso!\n");
      return;
    }
  }

  printf("Hospede com o CPF %s n�o encontrado.\n", cpfProcurado);
  fclose(arquivo);
}

void imprimirTodosCadastrados() {
  hospede hospedeAtual;
  FILE *arquivo = fopen("hospedes.dat", "rb");

  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de hospedes para leitura!\n");
    return;
  }

  printf("Hospedes cadastrados:\n");
  while (fread(&hospedeAtual, sizeof(hospede), 1, arquivo)) {
    if (hospedeAtual.cpf[0] != '\0') {
      printf("CPF: %s, Nome: %s %s, Email: %s, Telefone: %d\n",
             hospedeAtual.cpf, hospedeAtual.nome, hospedeAtual.sobrenome,
             hospedeAtual.email, hospedeAtual.telefone);
    }
  }

  fclose(arquivo);
}

void gerarRelatorio() {
  hospede hospedeAtual;
  FILE *arquivoEntrada = fopen("hospedes.dat", "rb");
  FILE *arquivoSaida = fopen("relatorio.txt", "w");

  if (arquivoEntrada == NULL) {
    printf("Erro ao abrir o arquivo de hospedes para leitura!\n");
    return;
  }

  if (arquivoSaida == NULL) {
    printf("Erro ao abrir o arquivo de relatorio para escrita!\n");
    fclose(arquivoEntrada);
    return;
  }

  fprintf(arquivoSaida, "Relatorio de Hospedes:\n");
  while (fread(&hospedeAtual, sizeof(hospede), 1, arquivoEntrada)) {
    if (hospedeAtual.cpf[0] != '\0') {
      fprintf(arquivoSaida, "CPF: %s, Nome: %s %s, Email: %s, Telefone: %d\n",
              hospedeAtual.cpf, hospedeAtual.nome, hospedeAtual.sobrenome,
              hospedeAtual.email, hospedeAtual.telefone);
    }
  }

  fclose(arquivoEntrada);
  fclose(arquivoSaida);
  printf("Relatorio gerado com sucesso!\n");
}
