public class Exercicio1 {
    public static void main(String[] args) {
        String[] URLs = {
                "https://www.dei.uc.pt/poao/exames",
                "http://www.scs.org/index.html",
                "https://www.nato.int/events",
                "https://www.btu.de/",
                "https://www.dei.uc.pt/poao/exames",
                "http://www.eth.ch/index.html",
                "http://www.osu.edu/"
        };

        String[][] paises = {
                {"pt", "Portugal"},
                {"org", "EUA"},
                {"fr", "França"},
                {"uk", "Reino Unido"},
                {"de", "Alemanha"},
                {"edu", "EUA"}
        };

        int repetidos = repetidos(paises);
        String[] PaisesUnicos = paisesUnicos(paises, repetidos);
        int[] Contador = contadorPaises(URLs, paises, PaisesUnicos);
        String[][] MatrizFinal = matrizFinal(Contador, PaisesUnicos);
        imprimir(MatrizFinal);
    }

    public static int repetidos(String[][] paises) { //este método retorna o número de paises repetidos
        int repetidos = 0;
        for (int i = 0; i < paises.length; i++) {
            for (int j = i + 1; j < paises.length; j++) {
                if (paises[i][1].equals(paises[j][1])) {
                    repetidos++;
                    break;
                }
            }
        }
        return repetidos;
    }

    public static String[] paisesUnicos(String[][] paises, int repetidos) {

        String[] PaisesUnicos = new String[paises.length - repetidos]; //guarda o nome dos países apenas uma vez para não haver repetições

        int k = 0;
        for (int i = 0; i < paises.length; i++) {
            boolean repetido = false; //verifica se o país já existe no array
            for (int j = 0; j < paises.length; j++) {
                if (i > j && paises[i][1].equals(paises[j][1])) {
                    repetido = true;
                    break;
                }
            }
            if (!repetido) {
                PaisesUnicos[k] = paises[i][1];
                k++;
            }
        }
        return PaisesUnicos;
    }

    public static int[] contadorPaises(String[] URLs, String[][] paises, String[] PaisesUnicos) {

        int[] Contador = new int[PaisesUnicos.length + 1]; //contador para cada país

        for (String url : URLs) {
            String[] auxiliar = url.split("//"); //separa os links nas duas barras
            auxiliar = auxiliar[1].split("/"); //separa os links na barra
            int indice = auxiliar[0].lastIndexOf(".") + 1; //recolhe o índice do primeiro caracter depois do último ponto final
            String pais = auxiliar[0].substring(indice); //cria uma string com as palavras que se encontram a partir desses índices (pt, org ...)

            boolean encontrado = false; //verifica se o país for encontrado
            for (int i = 0; i < paises.length; i++) {
                if (paises[i][0].equals(pais)) {
                    encontrado = true;
                    for (int j = 0; j < PaisesUnicos.length; j++) {
                        if (paises[i][1].equals(PaisesUnicos[j])) {
                            Contador[j]++; //se um país for encontrado aumenta o seu contador
                        }
                    }
                }
            }
            if (!encontrado) { //se o pais nao for encontrado...
                Contador[Contador.length - 1]++; //contador destinado a outros paises (que se encontra na ultima posição) incrementa
            }
        }

        return Contador;
    }

    public static String[][] matrizFinal(int[] Contador, String[] PaisesUnicos) {

        String[][] MatrizFinal = new String[PaisesUnicos.length + 1][2]; //cria uma matriz com o número de linhas igual ao número dos países + 1 (outros) e 2 colunas

        for (int i = 0; i < PaisesUnicos.length; i++) {
            MatrizFinal[i][0] = PaisesUnicos[i]; //copia o nome dos países para a primeira coluna da matriz
            MatrizFinal[i][1] = String.valueOf(Contador[i]); //copia o valor do contador (convertido em string) para a segunda coluna da matriz
        }
        MatrizFinal[MatrizFinal.length - 1][0] = "Outro(s)"; //adiciona outros na última posição da matriz
        MatrizFinal[MatrizFinal.length - 1][1] = String.valueOf(Contador[PaisesUnicos.length]); //copia o valor do contador outros para a última posição da matriz
        return MatrizFinal;
    }

    public static void imprimir(String[][] MatrizFinal) {
        for (int i = 0; i < MatrizFinal.length; i++) {
            if (!(MatrizFinal[i][1].equals("0"))) { //mostrar apenas os contadores nao nulos
                System.out.println(MatrizFinal[i][0] + ": " + MatrizFinal[i][1]);
            }
        }
    }
}