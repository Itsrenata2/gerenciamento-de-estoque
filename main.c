#include <stdio.h>
#include <stdlib.h> // biblioteca para funções de alocação de memória e controle de processos etc
#include <string.h> // biblioteca para manipulação de strings

#define MAX_ITENS 1000
#define MAX_STRING 50
#define MAX_DATA 11

struct ItensEstoque {
	char nomeProduto[MAX_STRING], tipoProduto[MAX_STRING], fornecedor[MAX_STRING];
	char dataRecebimento[MAX_DATA], dataVencimento[MAX_DATA];
	int quantDisponivel;
	int dia, mes, ano; 
	double precoUnitario;
};

int leArquivo(struct ItensEstoque itens[], int *contarItens);
void opcoes();
int verItens(struct ItensEstoque itens[], int *contarItens);
int addItens(struct ItensEstoque itens[], int *contarItens);
int delItens(struct ItensEstoque itens[], int *contarItens);
int atualizarItens(struct ItensEstoque itens[], int *contarItens);
int salvarDados(struct ItensEstoque itens[], int *contarItens);

int main() {
	struct ItensEstoque itens[MAX_ITENS];
	int contarItens = 0;

	leArquivo(itens, &contarItens);

	int escolhaUsuario;
	do {
	 	opcoes();
	 	printf("\nEscolha a ação que você quer realizar: ");
	 	scanf("%d", &escolhaUsuario);

	 	switch(escolhaUsuario) {
	 	case 1:
	 		verItens(itens, &contarItens);
	 		break;
	 	case 2:
	 		addItens(itens, &contarItens);
	 		break;
	 	case 3:
	 		delItens(itens, &contarItens);
	 		break;
	 	case 4:
	 		atualizarItens(itens, &contarItens);
	 		break;
	 	case 5:
	 		salvarDados(itens, &contarItens);
	 		break;
	 	}
	 } while (escolhaUsuario != 6); 
}

void opcoes() {
	printf("\n+++++++ GERENCIAMENTO DE ESTOQUE +++++++\n\n");
    printf("1. Ver Estoque\n");
    printf("2. Adicionar Produto\n");
    printf("3. Deletar Produto\n"); 
    printf("4. Atualizar Produto\n");
    printf("5. Salvar\n");
    printf("6. Exit\n");
}

// função que confere se atingiu a quantidade de itens estabelecida
int leArquivo(struct ItensEstoque itens[], int *contarItens) {
	// comando para ler o arquivo csv
	FILE* arquivo = fopen("sistemaestoque.csv", "r");

	// tratamento de erro, caso o arquivo não seja encontrado
	if (!arquivo) {
		printf("Não foi possível abrir o arquivo!\n");
		return 0;
	}

	while(fscanf(arquivo, "%49[^,], %49[^,], %d, %lf, %49[^,], %10[^,], %10[^,] \n", 
		itens[*contarItens].nomeProduto, 
		itens[*contarItens].tipoProduto, 
		&itens[*contarItens].quantDisponivel, 
		&itens[*contarItens].precoUnitario, 
		itens[*contarItens].fornecedor, 
		itens[*contarItens].dataRecebimento, 
		itens[*contarItens].dataVencimento) == 7) {

		(*contarItens)++;

		if(*contarItens >= MAX_ITENS) {
			printf("O número total de itens foi atingido!\n");
			break;
		}
	}

	fclose(arquivo);
	return 0;
}

// função para mostrar os itens do arquivo
int verItens(struct ItensEstoque itens[], int *contarItens) {
	if (contarItens == 0) {
		printf("Não tem itens no estoque...\n");
		return 0;
	}

	printf("\n---------------------------------------------------------- GERENCIAMENTO DE ESTOQUE --------------------------------------------------------\n\n");
	printf("    %-18s %-14s %-20s %-20s %-14s %-23s %-23s", "Nome do Produto", "Quantidade", "Preço Unitário", "Tipo do Produto", "Fornecedor", "Data de Recebimento", "Data de Vencimento");
	printf("\n--------------------------------------------------------------------------------------------------------------------------------------------\n");

	for (int i = 0; i < *contarItens; i++) {
		printf("    %-18s %-14d R$ %-20f %-20s %-14s %-23s %-23s\n", itens[i].nomeProduto, itens[i].quantDisponivel, itens[i].precoUnitario, itens[i].tipoProduto, itens[i].fornecedor, itens[i].dataRecebimento, itens[i].dataVencimento);
	}

	return 0;
}

// função para adicionar itens no arquivo
int addItens(struct ItensEstoque itens[], int *contarItens) {
    if (*contarItens >= MAX_ITENS) {
        printf("O número total de itens foi atingido! Não é possível adicionar mais itens.\n");
        return 0;
    }

    printf("Informe o nome do produto: ");
	scanf(" %49[^\n]", itens[*contarItens].nomeProduto);

	printf("Informe a quantidade disponível: ");
	scanf("%d", &itens[*contarItens].quantDisponivel);
	scanf("%*c");

	printf("Informe o preço unitário: ");
	scanf("%lf", &itens[*contarItens].precoUnitario);
	scanf("%*c");

	printf("Informe o tipo de produto: ");
	scanf(" %49[^\n]", itens[*contarItens].tipoProduto);

	printf("Informe o fornecedor: ");
	scanf(" %49[^\n]", itens[*contarItens].fornecedor);

	printf("Informe a data de recebimento do produto (formato dd/mm/yyyy): ");
    scanf("%10s", itens[*contarItens].dataRecebimento);

    printf("Informe a data de vencimento do produto (formato dd/mm/yyyy): ");
    scanf("%10s", itens[*contarItens].dataVencimento);

    (*contarItens)++;
    printf("\n+++++ Novo produto adicionado! +++++\n");

    return 0;
}

// função para deletar itens no arquivo
int delItens(struct ItensEstoque itens[], int *contarItens) {
	if (*contarItens == 0) {
        printf("Não existe itens para deletar...\n");
        return 0;
    }

    char nomeItem[MAX_STRING];
    printf("Informe o nome do produto que deseja excluir: ");
    scanf("%s", nomeItem);

    int index = -1;
    for (int i = 0; i < *contarItens; ++i) {
        if (strcmp(itens[i].nomeProduto, nomeItem) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Item não encontrado...\n");
    } else {
        for (int i = index; i < *contarItens - 1; ++i) {
            itens[i] = itens[i + 1];
        }
        (*contarItens)--;
        printf("Produto excluído...\n");
    }

    return 0;
}

// função para atualizar a quantidade de itens no arquivo
int atualizarItens(struct ItensEstoque itens[], int *contarItens) {
    if (contarItens == 0) {
        printf("Não existe itens para atualizar...\n");
        return 0;
    }

    char nomeItem[MAX_STRING];
    printf("Informe o nome do produto que deseja atualizar: ");
    scanf("%s", nomeItem);

    int index = -1;
    for (int i = 0; i < *contarItens; ++i) {
        if (strcmp(itens[i].nomeProduto, nomeItem) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Item não encontrado...\n");
    } else {
        printf("Insira nova quantidade: ");
        scanf("%d", &itens[index].quantDisponivel);
        printf("Quantidade atualizada...\n");
    }

    return 0;
}

// função para salvar o arquivo
int salvarDados(struct ItensEstoque itens[], int *contarItens) {
	FILE* arquivo = fopen("sistemaestoque.csv", "a");

	if (arquivo == NULL) {
		printf("Erro ao abrir arquivo...\n");
		return 0;
	} 

	for (int i = 0; i < *contarItens; ++i) {
		fprintf(arquivo, "%s, %d, R$ %2lf, %s, %s, %s, %s\n", itens[i].nomeProduto, itens[i].quantDisponivel, itens[i].precoUnitario, itens[i].tipoProduto, itens[i].fornecedor, itens[i].dataRecebimento, itens[i].dataVencimento);
	}

	fclose(arquivo);
	return 0;
}