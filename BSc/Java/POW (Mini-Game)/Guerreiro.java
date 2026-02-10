import java.util.Random;
class Guerreiro extends Champion{
    private boolean armadura;
    private String arma;

    Guerreiro(String nome, boolean armadura, String arma){ // Construtor para Guerreiro
        super(nome,new Random().nextInt(1,25+1),10,3,5);
        this.armadura = armadura;
        this.arma = arma;
    }

    public boolean getArmadura(){
        return armadura;
    }
    public String getArma(){
        return arma;
    }
    public void setArmadura(boolean armadura){
        this.armadura = armadura;
    }
    public void setArma(String arma){
        this.arma = arma;
    }
    public void imprimirEquip(){
        if (getArmadura()) {
            System.out.println("Nome: " + getNome() + " | " +
                    "Armadura: " + (getArmadura() ? "Possui" : "Não Possui") + " | " +
                    "Arma Curta: " + getArma());
        }
    }

    public void aumentarExperiencia() {
        int experiencia = getExperiencia() + 1;
        double forca = getForca() + getForca() * 0.2;
        double inteligencia = getInteligencia() + getInteligencia() * 0.05;
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