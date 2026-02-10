import java.util.Random;
class Mercenario extends Champion{
    private String arma;
    private int municoes;

    Mercenario(String nome, String arma, int municoes){ // Construtor para Mercenário
        super(nome,new Random().nextInt(1,25+1), 4,4,10);
        this.arma = arma;
        this.municoes = municoes;
    }

    public String getArma(){
        return arma;
    }
    public int getMunicoes(){
        return municoes;
    }
    public void setArma(String arma){
        this.arma = arma;
    }
    public void setMunicoes(int municoes){
        this.municoes = municoes;
    }

    public void imprimirEquip(){
        if (getArma().equals("Arcos")) {
            System.out.println("Nome: " + getNome() + " | " +
                    "Arma Longa: " + getArma() + " | " +
                    "Munições: " + getMunicoes());
        }
    }
    public void aumentarExperiencia() {
        int experiencia = getExperiencia() + 1;
        double forca = getForca() + getForca() * 0.08;
        double inteligencia = getInteligencia() + getInteligencia() * 0.08;
        double agilidade = getAgilidade() + getAgilidade() * 0.2;

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