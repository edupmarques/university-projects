abstract class Champion {
    private String nome;
    private int experiencia;
    private double forca;
    private double inteligencia;
    private double agilidade;

    public abstract void imprimirEquip(); // Função que imprime os mercenários com arcos, os magos com sementes de abóbora e os guerreiros com armadura
    public abstract void aumentarExperiencia(); // Função que aumenta as estatísticas dos Champions e atualiza-as utilizando os respetivos "set's"

    Champion(String nome, int experiencia, double forca, double inteligencia, double agilidade){ // Construtor para Champion
        this.nome = nome;
        this.experiencia = experiencia;
        this.forca = forca;
        this.inteligencia = inteligencia;
        this.agilidade = agilidade;
    }
    public String getNome(){
        return nome;
    }
    public int getExperiencia(){
        return experiencia;
    }
    public double getForca(){
        return forca;
    }
    public double getInteligencia(){
        return inteligencia;
    }
    public double getAgilidade(){
        return agilidade;
    }
    public void setNome(String nome){
        this.nome = nome;
    }
    public void setExperiencia(int experiencia){
        this.experiencia = experiencia;
    }
    public void setForca(double forca){
        this.forca = forca;
    }
    public void setInteligencia(double inteligencia){
        this.inteligencia = inteligencia;
    }
    public void setAgilidade(double agilidade){
        this.agilidade = agilidade;
    }

    public String toString() {
        return "Nome: " + getNome() + " | " + "Experiência: " + getExperiencia();
    } // Retorna uma string com o nome e a experiência de cada Champion
}