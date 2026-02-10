import java.util.ArrayList;
import java.util.Scanner;

class PoW {
    public static void imprimirChamps(ArrayList<Champion> champions) { // Recebe o ArrayList com todos os Champions
        if (!champions.isEmpty()) {  // Verifica se o ArrayList de champions está vazio
            System.out.println("\nLista de todos os Champions:"); // Se não estiver vazio, imprime os Champions que nela existem
            for (Champion champ : champions) {
                System.out.print(champ);
            }
        }
        else { // Se estiver vazio, imprime uma mensagem a alertar o utilizador do mesmo
            System.out.println("\nA lista de Champions ainda está vazia!");
        }
    }

    public static void maiorExperiencia(ArrayList<Champion> champions, int valor) { // Recebe o ArrayList com todos os Champions e um "valor" que é inserido pelo utilizador
        ArrayList<Champion> auxiliar = new ArrayList<>(); // ArrayList que guarda apenas os Champions com experiência superior ao "valor"

        for (Champion champ : champions) {
            if (champ.getExperiencia() > valor) { // Verifica se os Champions têm experiência superior ao "valor"
                auxiliar.add(champ); // Guarda-os no ArrayList "auxiliar"
            }
        }
        if (!auxiliar.isEmpty()) { // Se o ArrayList "auxiliar" não estiver vazio imprime os Champions que nela existem
            System.out.println("\nLista de Champions com experiência superior a " + valor + ":");
            for (Champion champ : auxiliar) {
                System.out.print(champ);
            }
        }
        else { // Se estiver vazio imprime uma mensagem a alertar o utilizador do mesmo
            System.out.println("\nNenhum Champion com experiência superior a " + valor + "!");
        }
    }

    public static void evoluir(ArrayList<Champion> champions, String nome) { // Recebe o ArrayList com todos os Champions e um "nome" que é inserido pelo utilizador
        ArrayList<Champion> auxiliar = new ArrayList<>(); // ArrayList que guarda apenas o Champion com nome igual ao nome que é inserido pelo utilizador
        for (Champion champ : champions) {
            if (champ.getNome().equals(nome)) { // Verifica se o nome do Champion é igual ao nome inserido pelo utilizador
                auxiliar.add(champ); // Guarda-os no ArrayList "auxiliar"
            }
        }
        if (!auxiliar.isEmpty()) { // Se o ArrayList "auxiliar" não estiver vazio imprime as estatísticas dos Champions que nela existem, , antes e depois da chamada da funçao "aumentarExperiencia"
            for (Champion champ : auxiliar) {
                System.out.println("\nEstatísticas antigas do Champion " + champ.getNome() + ": ");
                System.out.printf("Nome: %s | Experiência: %d | Força: %.2f | Inteligência: %.2f | Agilidade: %.2f \n",
                        champ.getNome(), champ.getExperiencia(), champ.getForca(), champ.getInteligencia(), champ.getAgilidade());

                if (champ.getExperiencia() < 25) {
                    champ.aumentarExperiencia();

                    System.out.println("\nEstatísticas novas do Champion " + champ.getNome() + ": ");
                    System.out.printf("Nome: %s | Experiência: %d | Força: %.2f | Inteligência: %.2f | Agilidade: %.2f \n",
                            champ.getNome(), champ.getExperiencia(), champ.getForca(), champ.getInteligencia(), champ.getAgilidade());

                }
                else{
                    System.out.println("\nNão pode evoluir mais... O Champion " + nome + " já está no nível máximo!");
                }
            }
        }
        else{ // Se estiver vazio imprime uma mensagem a alertar o utilizador do mesmo
            System.out.println("\nNão existe nenhum Champion chamado " + nome + "!");
        }
    }

    public static void imprimirEquip(ArrayList<Champion> champions) { // Recebe o ArrayList com todos os Champions
        System.out.println("\nLista de Champions com o equipamento que especificou: ");
        for (Champion champ : champions) {
            champ.imprimirEquip(); // Imprime apenas os mercenários com arcos, os magos com sementes de abóbora e os guerreiros com armadura
        }
    }

    public static void main(String[] args){
        Scanner scanner = new Scanner(System.in);
        ArrayList<Champion> champions = new ArrayList<>(); // Novo ArrayList que guarda todos os Champions

        int escolha;

        do {
            // Mostra o menu do jogo
            System.out.println("\n~~ POAO OF WARCRAFT ~~");
            System.out.println("1. Criar um novo Champion");
            System.out.println("2. Mostrar todos os Champions");
            System.out.println("3. Mostrar os Champions com experiência superiror a um dado valor");
            System.out.println("4. Mostrar os Champions com um determinado equipamento (Guerreiros com armadura, Magos com sementes de abóbora e Mercenários com arcos)");
            System.out.println("5. Evoluir um Champion");
            System.out.println("6. Sair do jogo");
            System.out.print("Escolha: ");
            escolha = scanner.nextInt();

            String novoNome; // String que guarda o nome do Champion a ser criado
            Champion novoChampion;

            switch (escolha) {
                case 1:
                    // Pede ao utilzador para escolher o tipo de Champion
                    System.out.println("~~ TIPO DE CHAMPION ~~");
                    System.out.println("1. Guerreiro");
                    System.out.println("2. Mago");
                    System.out.println("3. Mercenario");
                    System.out.print("Escolha: ");
                    int champEscolhido = scanner.nextInt();

                    switch (champEscolhido) {
                        case 1:
                            // No caso de ser guerreiro:
                            // Pede ao utilizador o nome do guerreiro
                            System.out.println("~~ NOME DO GUERREIRO ~~");
                            System.out.print("Nome: ");
                            novoNome = scanner.next();

                            // Pede ao utilizador se quer ou nao que o guerreiro tenha armadura
                            System.out.println("~~ ARMADURA ~~");
                            System.out.println("1. Sim");
                            System.out.println("2. Não");
                            System.out.print("Escolha: ");
                            int opcArmadura = scanner.nextInt();

                            boolean armadura = false; // Boolean que guarda a existência de armadura

                            switch(opcArmadura){ // Atualiza o boolean para true se o guerreiro tiver armadura
                                case 1:
                                    armadura = true;
                                    break;
                                case 2:
                                    armadura = false;
                                    break;
                                default:
                                    System.out.println("Opção inválida!");
                                    break;
                            }

                            // Pede ao utilizador o tipo de arma de curto alcance do guerreiro
                            System.out.println("~~ ARMA DE CURTO ALCANCE ~~");
                            System.out.println("1. Facas");
                            System.out.println("2. Machados");
                            System.out.println("3. Espadas");
                            System.out.print("Escolha: ");
                            int opcArmaC = scanner.nextInt();

                            String armaC = ""; // String vazia que guarda o nome da arma de curto alcance do guerreiro

                            switch(opcArmaC){ // Atualiza a String com o nome da arma escolhida
                                case 1:
                                    armaC = "Facas";
                                    break;
                                case 2:
                                    armaC = "Machados";
                                    break;
                                case 3:
                                    armaC = "Espadas";
                                    break;
                                default:
                                    System.out.println("Opção inválida!");
                                    break;
                            }
                            novoChampion = new Guerreiro(novoNome,armadura,armaC); // Cria um Champion do tipo guerreiro com os dados escolhidos pelo utilzador
                            System.out.println("\nAdicionou um Guerreiro chamado " + novoNome + " !"); // Imprime uma mensagem a alertar o utilizador da criação do guerreiro
                            break;
                        case 2:
                            // No caso de ser mago:
                            // Pede ao utilizador o nome do mago
                            System.out.println("~~ NOME DO MAGO ~~");
                            System.out.print("Nome: ");
                            novoNome = scanner.next();

                            String sementes; // String vazia que guarda o nome das sementes
                            String folhas; // String vazia que guarda o nome das folhas

                            // Pede ao utilizador qual o conteúdo da mochila do mago
                            System.out.println("~~ MOCHILA ~~");
                            System.out.println("A mochila contém sementes e folhas!");
                            System.out.print("Insira o tipo de sementes: "); // Pede ao utilizador o tipo de sementes (String)
                            sementes = scanner.next();

                            System.out.print("Insira o tipo de folhas: "); // Pede ao utilizador o tipo de folhas (String)
                            folhas = scanner.next();

                            novoChampion = new Mago(novoNome, sementes, folhas); // Cria um Champion do tipo mago com os dados escolhidos pelo utilzador
                            System.out.println("\nAdicionou um Mago chamado " + novoNome + " !"); // Imprime uma mensagem a alertar o utilizador da criação do mago
                            break;
                        case 3:
                            // No caso de ser mercenário:
                            // Pede ao utilizador o nome do mercenário
                            System.out.println("~~ NOME DO MERCENÁRIO ~~");
                            System.out.print("Nome: ");
                            novoNome = scanner.next();

                            // Pede ao utilizador o tipo de arma de longo alcance do mercenário
                            System.out.println("~~ ARMA DE LONGO ALCANCE ~~");
                            System.out.println("1. Pedras");
                            System.out.println("2. Arcos");
                            System.out.println("3. Pistolas");
                            System.out.print("Escolha: ");
                            int opcArmaL = scanner.nextInt();

                            String armaL = ""; // String vazia que guarda o nome da arma de longo alcance do mercenário

                            switch(opcArmaL) { // Atualiza a String com o nome da arma escolhida
                                case 1:
                                    armaL = "Pedras";
                                    break;
                                case 2:
                                    armaL = "Arcos";
                                    break;
                                case 3:
                                    armaL = "Pistolas";
                                    break;
                                default:
                                    System.out.println("Opção inválida!");
                                    break;
                            }

                            // Pede ao utilizador a quantidade (número inteiro) de munição da arma escolhida
                            System.out.println("~~ MUNIÇÃO ~~");
                            System.out.print("Quantidade: ");
                            int municao = scanner.nextInt();
                            novoChampion = new Mercenario(novoNome,armaL,municao); // Cria um Champion do tipo mercenário com os dados escolhidos pelo utilzador
                            System.out.println("\nAdicionou um Mercenário chamado " + novoNome + " !"); // Imprime uma mensagem a alertar o utilizador da criação do mercenário
                            break;
                        default:
                            System.out.println("Opção inválida!");
                            continue;
                    }
                    champions.add(novoChampion); // Adiciona o novo Champion ao ArrayList champions
                    break;
                case 2:
                    imprimirChamps(champions); // Chama a função que imprime todos os Champions
                    break;
                case 3:
                    System.out.print("Escolha esse valor: "); // Pede ao utilazador um valor (número inteiro)
                    int valor = scanner.nextInt();
                    maiorExperiencia(champions,valor); // Chama a função que imprime os Champions com experiência superior ao valor inserido pelo utilizador
                    break;
                case 4:
                    imprimirEquip(champions); // Chama a função que imprime apenas os mercenários com arcos, os magos com sementes de abóbora e os guerreiros com armadura
                    break;
                case 5:
                    System.out.print("Insira o nome do Champion que quer evoluir: "); // Pede ao utilizador o nome (string) do Champion que quer evoluir
                    String nome = scanner.next();
                    evoluir(champions,nome); // Chama a função que imprime as estatísticas antes e depois da evolução do Champion
                    break;
                case 6:
                    System.out.println("A sair do jogo..."); // Imprime uma mensagem a alertar o utilizador que está a sair do jogo
                    break;
                default:
                    System.out.println("Opção inválida!");
            }

        } while (escolha != 6);
        scanner.close();
    }
}