import java.util.Random;
class Mago extends Champion{
    private String sementes;
    private String folhas;

    Mago(String nome, String sementes, String folhas){ // Construtor para Mago
        super(nome, new Random().nextInt(1,25+1),2,9,4);
        this.sementes = sementes;
        this.folhas = folhas;
    }

    public String getSementes(){
        return sementes;
    }

    public String getFolhas(){
        return folhas;
    }

   public void setSementes(String sementes){
        this.sementes = sementes;
   }

   public void setFolhas(String folhas){
        this.folhas = folhas;
   }
    public void imprimirEquip(){
        if (getSementes().equals("abóbora")) {
            System.out.println("Nome: " + getNome() + " | " +
                    "Mochila: " + "Sementes de " + getSementes() + " e folhas de " + getFolhas());
        }
    }
    public void aumentarExperiencia() {
        int experiencia = getExperiencia() + 1;
        double forca = getForca() + getForca() * 0.05;
        double inteligencia = getInteligencia() + getInteligencia() * 0.2;
        double agilidade = getAgilidade() + getAgilidade() * 0.1;

        // Atualiza os valores das estatísticas após o aumento das mesmas
        setExperiencia(experiencia);
        setForca(forca);
        setInteligencia(inteligencia);
        setAgilidade(agilidade);
    }

    public String toString() {
        return super.toString() + "\n";
    } // Retorna uma string com o nome e a experiência de cada Champion (definido na class Champion)
}