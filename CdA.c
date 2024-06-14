#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

//Definicao das constantes
#define MAX_PACIENTES 100
#define MAX_EXAMES 50
#define DIAS_JUNHO 36
#define HORARIOS_DIA 7
#define DIAS_SEMANA 7
#define COR_VERMELHO "\x1b[31m"
#define COR_VERDE "\x1b[32m"
#define COR_RESET "\x1b[0m"

struct Exame {
    char nome[50];
    float valor;
    int prazo;
};

//Estrutura de cadastro dos dados pessoais do paciente, exames, precos, prazos e visita.
struct cadastro {
    char nome_completo[30];
    char cpf[12];
    char data_nascimento[12];
    char telefone[12];
    char email[20];
    char endereco[40];
    char visita[50];
    struct Exame exames_agendados[MAX_EXAMES];
    int num_exames_agendados;
    struct Exame novos_exames[MAX_EXAMES];
    int num_novos_exames;
};

//Estrutura com a lista de exames para validacao
struct Exame exames[] = {
        {"Hemograma", 50.00, 2},
        {"Colesterol", 40.00, 1},
        {"Glicemia", 30.00, 1},
        {"TSH", 45.00, 3},
        {"T4L", 50.00, 3},
        {"T3T", 55.00, 3},
        {"PCR", 40.00, 1},
        {"Acido Urico", 35.00, 1},
        {"Creatinina", 30.00, 1},
        {"Ureia", 25.00, 1},
        {"B9", 50.00, 3},
        {"B12", 55.00, 3},
        {"Ferritina", 60.00, 3},
        {"PSA", 65.00, 3},
        {"TGO", 40.00, 1},
        {"TGP", 40.00, 1},
        {"FA", 35.00, 1},
        {"Bilirrubinas", 30.00, 1},
        {"Amilase", 45.00, 2},
        {"Lipase", 50.00, 2},
        {"Cloro", 25.00, 1},
        {"Sodio", 30.00, 1},
        {"Potassio", 35.00, 1},
        {"Calcio", 40.00, 1},
        {"Magnesio", 45.00, 2},
        {"Fosforo", 30.00, 1},
        {"CKMB", 55.00, 3},
        {"TP", 35.00, 1},
        {"TTPA", 40.00, 1},
        {"HG", 45.00, 2},
        {"EAS", 30.00, 1},
        {"Parasitologico", 25.00, 1},
        {"Beta-hCG", 30.00, 1},
        {"HIV", 40.00, 1},
        {"HBSAG", 45.00, 1},
        {"Anti HCV", 50.00, 1},
        {"VDRL", 35.00, 1},
        {"Rubeola", 35.00, 1},
        {"Toxo", 40.00, 1},
        {"CMV", 45.00, 1},
        {"Herpes", 50.00, 1},
        {"Dengue", 55.00, 1},
        {"Zika", 60.00, 1},
        {"Chikungunya", 65.00, 1}
};

//Limpeza de buffer
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
//Tentativa para remover a linha
void remover_nova_linha(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Array para marcar horários indisponíveis
bool horarios_disponiveis[DIAS_JUNHO][HORARIOS_DIA];

// Função para definir a cor vermelha no terminal
void setRed() {
    printf(COR_VERMELHO);
}

// Função para definir a cor verde no terminal
void setGreen() {
    printf(COR_VERDE);
}

// Função para resetar a cor no terminal
void resetColor() {
    printf(COR_RESET);
}

void cadastro(struct cadastro pacientes[], int *num_pacientes);
void menu(struct cadastro pacientes[], int *num_pacientes);
void localizar_paciente(struct cadastro pacientes[], int num_pacientes);
void calendarioJunho();
void horariosDia(int dia);
void verificarAgendamentosDia(struct cadastro pacientes[], int num_pacientes, int dia);
void editar_paciente(struct cadastro pacientes[], int num_pacientes);

//
int main() {
    struct cadastro pacientes[MAX_PACIENTES];
    int num_pacientes = 0;

    // Inicializando todos os horários como disponíveis
    for (int i = 0; i < DIAS_JUNHO; i++) {
        for (int j = 0; j < HORARIOS_DIA; j++) {
            horarios_disponiveis[i][j] = true;
        }
    }

    menu(pacientes, &num_pacientes);
    return 0;
}
//Funcao do menu principal
void menu(struct cadastro pacientes[], int *num_pacientes) {
    int a;

    printf("\nSeja bem-vindo(a) ao CdA - Canal de Agendamentos. Digite o numero correspondente.\n\n");

    while (1) {
        
        printf("1 - Cadastrar novo paciente.\n2 - Localizar paciente.\n3 - Agendamentos.\n4 - Ver todos os agendamentos.\n5 - Sair.\n\n");
        scanf("%d", &a);
        limpar_buffer();

        system("CLS");  // Limpar a tela

        int dia_verificar;

        switch (a) {
            case 1:
                cadastro(pacientes, num_pacientes);
                break;
            case 2:
                editar_paciente(pacientes, *num_pacientes);
                break;
            case 3:
                localizar_paciente(pacientes, *num_pacientes);
                break;
            case 4:
                printf("Digite o dia a verificar (1-%d): ", DIAS_JUNHO-6);
                scanf("%d", &dia_verificar);
                limpar_buffer();

                if (dia_verificar < 1 || dia_verificar > DIAS_JUNHO-6) {
                    printf("Dia invalido. Por favor, escolha um dia entre 1 e %d.\n", DIAS_JUNHO-6);
                    break; // Saia do switch
                }

                verificarAgendamentosDia(pacientes, *num_pacientes, dia_verificar);
                break;
            case 5:
                printf("Saindo do programa.\n");
                return;
            default:
                printf("Digite uma opcaoo valida.\n");
        }
    }
}

//Funcao que cadastra o paciente.
void cadastro(struct cadastro pacientes[], int *num_pacientes) {
    if (*num_pacientes >= MAX_PACIENTES) {
        printf("Limite de pacientes cadastrados atingido. Sem agendas no momento.\n");
        return;
    }

    struct cadastro *pessoa = &pacientes[*num_pacientes];
    printf("**********************************************\n");
    printf("*           CADASTRO DE PACIENTE             *\n");
    printf("**********************************************\n");
    
    printf("*   Digite o Nome Completo:      ");
    fgets(pessoa->nome_completo, sizeof(pessoa->nome_completo), stdin);
    pessoa->nome_completo[strcspn(pessoa->nome_completo, "\n")] = '\0'; // Remover a quebra de linha
    
    printf("*   Digite o CPF:                ");
    fgets(pessoa->cpf, sizeof(pessoa->cpf), stdin);
    pessoa->cpf[strcspn(pessoa->cpf, "\n")] = '\0';
    limpar_buffer();

    printf("*   Data de Nascimento:          ");
    fgets(pessoa->data_nascimento, sizeof(pessoa->data_nascimento), stdin);
    pessoa->data_nascimento[strcspn(pessoa->data_nascimento, "\n")] = '\0';
    limpar_buffer();

    printf("*   Telefone Para Contato:       ");
    fgets(pessoa->telefone, sizeof(pessoa->telefone), stdin);
    pessoa->telefone[strcspn(pessoa->telefone, "\n")] = '\0';
    limpar_buffer();

    printf("*   E-mail Para Contato:         ");
    fgets(pessoa->email, sizeof(pessoa->email), stdin);
    pessoa->email[strcspn(pessoa->email, "\n")] = '\0';
    limpar_buffer();

    printf("*   Endereco Completo com CEP:   ");
    fgets(pessoa->endereco, sizeof(pessoa->endereco), stdin);
    pessoa->endereco[strcspn(pessoa->endereco, "\n")] = '\0';
    limpar_buffer();

    system("CLS");

    printf("**********************************************\n");
    printf("*            PACIENTE CADASTRADO             *\n");
    printf("**********************************************\n");

    int opcao;
    printf("\nDeseja efetuar um agendamento ou voltar ao menu inicial?\n");
    printf("1 - Efetuar Agendamento\n2 - Voltar ao Menu Inicial\n");
    scanf("%d", &opcao);
    limpar_buffer();
    system("CLS");

    if (opcao == 1) {

    //Seleciona exames    
    char examesSelecionados[100];
    printf("*   Selecione os exames desejados, separados por virgula: ");
    fgets(examesSelecionados, sizeof(examesSelecionados), stdin);
    examesSelecionados[strcspn(examesSelecionados, "\n")] = '\0';

    char* token = strtok(examesSelecionados, ", ");
    while (token != NULL) {
        // Verificar se o exame selecionado é válido
        bool exameValido = false;
        for (size_t i = 0; i < sizeof(exames) / sizeof(exames[0]); i++) {
            if (strcmp(token, exames[i].nome) == 0) {
                exameValido = true;
                break;
            }
        }
        if (!exameValido) {
            printf("*   Exame invalido: %s\n", token);
            examesSelecionados[0] = '\0'; // Limpar os exames selecionados para permitir nova seleção
            break;
        }
        token = strtok(NULL, ", ");
    }


    if (examesSelecionados[0] != '\0') {
        token = strtok(examesSelecionados, ", ");
        int num_exames_agendados = 0;
        while (token != NULL) {
            // Adicionar os exames selecionados à estrutura do paciente
            for (size_t i = 0; i < sizeof(exames) / sizeof(exames[0]); i++) {
                if (strcmp(token, exames[i].nome) == 0) {
                    strcpy(pessoa->exames_agendados[num_exames_agendados].nome, token);
                    pessoa->exames_agendados[num_exames_agendados].valor = exames[i].valor;
                    pessoa->exames_agendados[num_exames_agendados].prazo = exames[i].prazo;
                    num_exames_agendados++;
                    break;
                }
            }
            token = strtok(NULL, ", ");
        }
        pessoa->num_exames_agendados = num_exames_agendados;
    }
}
                int dia_escolhido;
        int horario_selecionado;

        calendarioJunho();
        
        printf("\nEscolha um dia em Junho (1-%d): ", DIAS_JUNHO-6);
        scanf("%d", &dia_escolhido);

        if (dia_escolhido >= 1 && dia_escolhido <= DIAS_JUNHO) {
            horariosDia(dia_escolhido);
            printf("\nEscolha um horario (1-7): ");
            scanf("%d", &horario_selecionado);

            if (horario_selecionado >= 1 && horario_selecionado <= HORARIOS_DIA) {
                if (horarios_disponiveis[dia_escolhido - 1][horario_selecionado - 1]) {
                    horarios_disponiveis[dia_escolhido - 1][horario_selecionado - 1] = false;
                    snprintf(pessoa->visita, sizeof(pessoa->visita), "\n#Visita agendada para o dia %d de Junho, das %02d:00 as %02d:00.\n\n", dia_escolhido, 6 + horario_selecionado - 1, 7 + horario_selecionado - 1);
                    printf("\n# Visita agendada para o dia %d de Junho, das %02d:00 as %02d:00.\n\n", dia_escolhido, 6 + horario_selecionado - 1, 7 + horario_selecionado - 1);
                    getchar();
                } else {
                    printf("Horario indisponivel!\n");
                }
            } else {
                printf("Horario invalido!\n");
            }
        } else {
            printf("Dia invalido!\n");
        }

        (*num_pacientes)++;
    }

void localizar_paciente(struct cadastro pacientes[], int num_pacientes) {
    int opcao_busca;
    char valor_busca[100];

    printf("Digite a opcao de busca (1 - Nome, 2 - CPF): ");
    scanf("%d", &opcao_busca);
    limpar_buffer();

    if (opcao_busca == 1) {
        printf("Digite o nome do paciente para localizar: ");
        fgets(valor_busca, sizeof(valor_busca), stdin);
        remover_nova_linha(valor_busca);
    } else if (opcao_busca == 2) {
        printf("Digite o CPF do paciente para localizar: ");
        fgets(valor_busca, sizeof(valor_busca), stdin);
        remover_nova_linha(valor_busca);
    } else {
        printf("Opcao de busca invalida.\n");
        return;
    }

    for (int i = 0; i < num_pacientes; i++) {
        if ((opcao_busca == 1 && strcmp(valor_busca, pacientes[i].nome_completo) == 0) ||
            (opcao_busca == 2 && strcmp(valor_busca, pacientes[i].cpf) == 0)) {
            printf("*****************************************\n");
            printf("*   Paciente encontrado:\n");
            printf("*   Nome: %s\n", pacientes[i].nome_completo);
            printf("*   CPF: %s\n", pacientes[i].cpf);
            printf("*   Telefone: %s\n", pacientes[i].telefone);
            printf("*   Email: %s\n", pacientes[i].email);
            printf("*   Endereco: %s\n", pacientes[i].endereco);
            printf("*   %s\n", pacientes[i].visita);
             printf("*****************************************\n\n");

            // Exibir exames agendados
            printf("Exames Agendados:\n");
            for (int j = 0; j < pacientes[i].num_exames_agendados; j++) {
                printf("Exame: %s\n", pacientes[i].exames_agendados[j].nome);
                printf("Preco: %.2f\n", pacientes[i].exames_agendados[j].valor);
                printf("Prazo: %d dias\n", pacientes[i].exames_agendados[j].prazo);
            }

            return;  // Paciente encontrado, encerra a função
        }
    }

    // Se chegou aqui, o paciente não foi encontrado
    printf("Paciente nao encontrado.\n");
}



void editar_paciente(struct cadastro pacientes[], int num_pacientes) {
    int opcao_busca;
    char valor_busca[100];

    printf("Digite a opcao de busca (1 - Nome, 2 - CPF): ");
    scanf("%d", &opcao_busca);
    limpar_buffer();

    if (opcao_busca == 1) {
        printf("Digite o nome do paciente para localizar: ");
        fgets(valor_busca, sizeof(valor_busca), stdin);
        valor_busca[strcspn(valor_busca, "\n")] = '\0';
    } else if (opcao_busca == 2) {
        printf("Digite o CPF do paciente para localizar: ");
        fgets(valor_busca, sizeof(valor_busca), stdin);
        valor_busca[strcspn(valor_busca, "\n")] = '\0';
    } else {
        printf("Opção de busca inválida.\n");
        return;
    }

    // Verificar se o paciente existe
    int indice_paciente = -1;
    for (int i = 0; i < num_pacientes; i++) {
        if ((opcao_busca == 1 && strcmp(valor_busca, pacientes[i].nome_completo) == 0) ||
            (opcao_busca == 2 && strcmp(valor_busca, pacientes[i].cpf) == 0)) {
            indice_paciente = i;
            break;
        }
    }

    if (indice_paciente == -1) {
        printf("Paciente não encontrado.\n");
        return;
    }

    int escolha_edicao;
    printf("Paciente encontrado. O que deseja fazer?\n");
    printf("1 - Editar paciente\n");
    printf("2 - Voltar ao Menu Inicial\n");
    printf("3 - Sair\n");
    scanf("%d", &escolha_edicao);
    limpar_buffer();

    switch (escolha_edicao) {
        case 1:
            // Chamar função para editar o paciente
            break;
        case 2:
            menu(pacientes, &num_pacientes);
            break;
        case 3:
            printf("Saindo do programa.\n");
            exit(0);
        default:
            printf("Opção inválida.\n");
            break;
    }

    fgets(valor_busca, sizeof(valor_busca), stdin);
    valor_busca[strcspn(valor_busca, "\n")] = '\0';

    int i;
    for (i = 0; i < num_pacientes; i++) {
        if ((opcao_busca == 1 && strcmp(valor_busca, pacientes[i].nome_completo) == 0) ||
            (opcao_busca == 2 && strcmp(valor_busca, pacientes[i].cpf) == 0)) {
            printf("*****************************************\n");
            printf("*   \nPaciente encontrado:\n\n");
            printf("*   Nome Completo: %s\n", pacientes[i].nome_completo);
            printf("*   CPF: %s\n", pacientes[i].cpf);
            printf("*   Telefone: %s\n", pacientes[i].telefone);
            printf("*   E-mail: %s\n", pacientes[i].email);
            printf("*   Endereço: %s\n", pacientes[i].endereco);
            printf("*   Exames Agendados:\n");
            printf("*****************************************\n\n");
            float valor_total = 0;
            int prazo_maximo = 0;
            for (int j = 0; j < pacientes[i].num_exames_agendados; j++) {
                printf("%s \n", pacientes[i].exames_agendados[j].nome);
                valor_total += pacientes[i].exames_agendados[j].valor;
                printf("Valor total: R$  %.2f",  pacientes[i].exames_agendados[j].valor);
                printf("Prazo: %d dias", pacientes[i].exames_agendados[j].prazo);
                if (pacientes[i].exames_agendados[j].prazo > prazo_maximo) {
                    prazo_maximo = pacientes[i].exames_agendados[j].prazo;
                }
            }
            printf("\nValor Total dos Exames: R$ %.2f\n", valor_total);
            printf("Prazo Maximo entre os Exames: %d dias\n", prazo_maximo);
            printf("\n\n");

            // Adicionar novos exames
            char examesSelecionados[100];
            printf("*   Adicione novos exames, separados por virgula: ");
            fgets(examesSelecionados, sizeof(examesSelecionados), stdin);
            examesSelecionados[strcspn(examesSelecionados, "\n")] = '\0';

            char* token = strtok(examesSelecionados, ", ");
            while (token != NULL) {
                // Verificar se o exame selecionado é válido
                bool exameValido = false;
                for (size_t k = 0; k < sizeof(exames) / sizeof(exames[0]); k++) {
                    if (strcmp(token, exames[k].nome) == 0) {
                        exameValido = true;
                        break;
                    }
                }
                if (!exameValido) {
                    printf("*   Exame inválido: %s\n", token);
                    examesSelecionados[0] = '\0'; // Limpar os exames selecionados para permitir nova seleção
                    break;
                }
                token = strtok(NULL, ", ");
            }

            if (examesSelecionados[0] != '\0') {
                token = strtok(examesSelecionados, ", ");
                int num_exames_agendados = pacientes[i].num_exames_agendados;
                while (token != NULL) {
                    // Adicionar os exames selecionados à estrutura do paciente
                    for (size_t k = 0; k < sizeof(exames) / sizeof(exames[0]); k++) {
                        if (strcmp(token, exames[k].nome) == 0) {
                            strcpy(pacientes[i].exames_agendados[num_exames_agendados].nome, token);
                            pacientes[i].exames_agendados[num_exames_agendados].valor = exames[k].valor;
                            pacientes[i].exames_agendados[num_exames_agendados].prazo = exames[k].prazo;
                            num_exames_agendados++;
                            break;
                        }
                    }
                    token = strtok(NULL, ", ");
                }
                pacientes[i].num_exames_agendados = num_exames_agendados;
            }
            return;
        }
    }
    printf("Paciente não encontrado.\n");
}


void calendarioJunho() {
    char dias[DIAS_JUNHO][3];
    int dia_inicio = 6;

    for (int i = 0; i < DIAS_JUNHO; i++) {
        if (i < dia_inicio) {
            snprintf(dias[i], sizeof(dias[i]), "  ");
        } else {
            snprintf(dias[i], sizeof(dias[i]), "%2d", i - dia_inicio + 1);
        }
    }

    printf("\nCalendario de Junho\n");
    printf("-------------------\n");
    printf(" Dom Seg Ter Qua Qui Sex Sab\n");

    for (int i = 0; i < DIAS_JUNHO; i++) {
        if (i % DIAS_SEMANA == 0) {
            printf(" ");
        }
        printf(" %s ", dias[i]);
        if ((i + 1) % DIAS_SEMANA == 0 || i == DIAS_JUNHO - 1) {
            printf("\n");
        }
    }
}

void horariosDia(int dia) {
    printf("\nHorarios para o dia %d de Junho:\n", dia);
    printf("--------------------\n");
    for (int i = 0; i < HORARIOS_DIA; i++) {
        if (horarios_disponiveis[dia - 1][i]) {
            setGreen();
        } else {
            setRed();
        }
        printf("%d -> %02d:00 - %02d:00 ",i+1, 6 + i, 7 + i);
        resetColor();
        printf("|\n");
    }
}

void verificarAgendamentosDia(struct cadastro pacientes[], int num_pacientes, int dia) {
    bool agendamentoEncontrado = false;

    system("CLS");
    printf("\nAgendamentos para o dia %d de Junho:\n", dia);
    printf("-----------------------------------\n");

    char data[20];
    snprintf(data, sizeof(data), "dia %d de Junho", dia);

    for (int i = 0; i < num_pacientes; i++) {
        if (strstr(pacientes[i].visita, data) != NULL) {
            printf("*****************************************\n");
            printf("#   Nome Completo: %s                   \n", pacientes[i].nome_completo);
            printf("#   CPF: %s                             \n", pacientes[i].cpf);
            printf("#   Telefone: %s                        \n", pacientes[i].telefone);
            printf("#   E-mail: %s                          \n", pacientes[i].email);
            printf("#   Endereço: %s                        \n", pacientes[i].endereco);
            printf("#   Horario: %s                        \n", pacientes[i].visita);
            printf("*****************************************\n\n");

            agendamentoEncontrado = true;
        }
    }

    if (!agendamentoEncontrado) {
        printf("Nenhum agendamento encontrado para o dia %d de Junho.\n\n", dia);
    }
}