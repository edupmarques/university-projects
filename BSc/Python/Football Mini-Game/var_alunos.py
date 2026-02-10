import foosball_alunos

def le_replay(nome_ficheiro):
    
    dicio = {}
    coordenadas_bola = []
    coordenadas_jogador_v = []
    coordenadas_jogador_a = []
    pontos = []
    
    with open(nome_ficheiro,'r') as fich:
        for linha in fich.readlines():
            pontos.append(linha[:-2].split(";")) 
    
    for i in pontos[0]:
        coordenadas_bola.append(tuple(map(float,(i.split(',')))))
    for i in pontos[1]:
        coordenadas_jogador_v.append(tuple(map(float,(i.split(',')))))
    for i in pontos[2]:
        coordenadas_jogador_a.append(tuple(map(float,(i.split(',')))))  
    
    dicio['bola'] = coordenadas_bola
    dicio['jogador_vermelho'] = coordenadas_jogador_v
    dicio['jogador_azul'] = coordenadas_jogador_a

    return dicio

def main():
    estado_jogo = foosball_alunos.init_state()
    foosball_alunos.setup(estado_jogo, False)
    replay = le_replay('replay_golo_jv_0_ja_0.txt')
    for i in range(len(replay['bola'])):
        estado_jogo['janela'].update()
        estado_jogo['jogador_vermelho'].setpos(replay['jogador_vermelho'][i])
        estado_jogo['jogador_azul'].setpos(replay['jogador_azul'][i])
        estado_jogo['bola']['bola'].setpos(replay['bola'][i])
    estado_jogo['janela'].exitonclick()

if __name__ == '__main__':
    main()