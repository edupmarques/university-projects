%% Exercícios

ficheiroPasta = "C:\ATD\05";
listaAudios = dir(fullfile(ficheiroPasta, '*.wav'));

%% Exercício 1

arrayAudios = cell(1, length(listaAudios));
arrayAudios = lerAudios(ficheiroPasta,listaAudios,arrayAudios);

%% Exercício 2

plotExemplos(arrayAudios);

%% Exercício 3

arrayFinal = cell(1, length(arrayAudios));
arrayFinal = diferenciacao(arrayAudios,arrayFinal);

%% Exercício 4

arrayDesvio = zeros(50,10);
arrayEnergia = zeros(50,10);
arrayAmplitude = zeros(50,10);
[arrayDesvio,arrayEnergia,arrayAmplitude] = calcularDados(arrayFinal,arrayDesvio,arrayEnergia,arrayAmplitude);
grafico(arrayDesvio,'Desvio Padrão','green');
grafico(arrayEnergia,'Energia','red');
grafico(arrayAmplitude,'Razão de Amplitudes','magenta');

%% Exercício 5 e 6

arrayFourierMedian = zeros(750, 10);
arrayQuartil25 = zeros(750,10);
arrayQuartil75 = zeros(750,10);
arrayMax = zeros(50,10);
arrayMin = zeros(50,10);
arrayMedia = zeros(50,10);
arrayfreqEspectral = zeros(50,10);
arrayMedianas = zeros(100,10);
[arrayFourierMedian,arrayQuartil25,arrayQuartil75,arrayMax,arrayMin,arrayMedia,arrayfreqEspectral] = fourier(arrayFinal,arrayFourierMedian,arrayQuartil25,arrayQuartil75,arrayMax,arrayMin,arrayMedia,arrayfreqEspectral,"blackman");

%% Exercício 7 e 8

grafico(arrayMax,'Max Espectrais','red');
grafico(arrayMin,'Min Espectrais','blue');
grafico(arrayMedia,'Médias Espectrias','magenta');
grafico(arrayfreqEspectral,'Spectral Edge Frequency','green');

%% Exercício 9

arrayMeanSpectr = zeros(50,10);
arrayVarSpectr = zeros(50,10);
arrayMaxSpectr = zeros(50,10);
arrayMedianSpectr = zeros(50,10);
[arrayMeanSpectr,arrayVarSpectr,arrayMaxSpectr,arrayMedianSpectr] = spectro(arrayFinal,arrayMeanSpectr,arrayVarSpectr,arrayMaxSpectr,arrayMedianSpectr);

%% Exercício 10 e 11

grafico(arrayMeanSpectr,'Médias de Potencias','red');
grafico(arrayVarSpectr,'Variâncias de Potencias','blue');
grafico(arrayMaxSpectr,'Max Potencias','green');
grafico(arrayMedianSpectr,'Medianas Potencias','magenta');

%% Exercício 12 e 13

digitosContador = zeros(1,500);
[digitosContador] = diferenciar(digitosContador,arrayMax,arrayDesvio,arrayfreqEspectral,arrayMedia);
[acertos] = calculo(arrayAudios,digitosContador);
fprintf("\n\nAcertos : %f%%\n",acertos);

%% Exercício 14

grafico3D (arrayAudios, arrayMax, arrayDesvio, arrayfreqEspectral);

%% Funções

function arrayAudios = lerAudios(ficheiroPasta,listaAudios,arrayAudios)
    for i = 1:length(listaAudios)
        nomeAudio = listaAudios(i).name;
        ficheiroAudio = fullfile(ficheiroPasta, nomeAudio);
        [sinal, freqAmostra] = audioread(ficheiroAudio);
        arrayAudios{i} = {nomeAudio, sinal, freqAmostra};
        fprintf('Áudio %s importado com sucesso!\n',nomeAudio);
    end
end

function [] = plotExemplos(arrayAudios)
    indicePlot = 1;
    figure;
    for i = 5:50:length(arrayAudios)
        sinal = arrayAudios{i}{2};
        freqAmostra = arrayAudios{i}{3};
        tempo = (0:size(sinal, 1)-1) / freqAmostra;
        subplot(5,2,indicePlot);
        plot(tempo, sinal);
        title(indicePlot-1);
        xlabel('Tempo [s]');
        indicePlot = indicePlot + 1;
    end
end

function sinalFinal = duracao(sinal,duracaoPretendida,freqAmostra)
    if length(sinal) < duracaoPretendida * freqAmostra
        sinalFinal = [sinal; zeros(duracaoPretendida * freqAmostra - length(sinal), 1)];
    else
        sinalFinal = sinal(1:duracaoPretendida * freqAmostra);
    end
end

function arrayFinal = diferenciacao(arrayAudios,arrayFinal)
    for i = 1:length(arrayAudios)
        nomeAudio = arrayAudios{i}{1};
        sinal = arrayAudios{i}{2};
        freqAmostra = arrayAudios{i}{3};
        energia = sum(abs(sinal).^2,2); % calcula a energia de cada linha da matriz sinal
        indices = find(energia > 0.05 * max(energia));
        sinal = sinal(indices(1):indices(length(indices)-1));
        amplitudeMin = min(abs(sinal));
        amplitudeMax = max(abs(sinal));
        sinal = (sinal - amplitudeMin) / (amplitudeMax - amplitudeMin); %normalizar o sinal
        sinalFinal = duracao(sinal,0.4,freqAmostra);
        arrayFinal{i} = {nomeAudio, sinalFinal,freqAmostra};
    end
end

function [arrayDesvio,arrayEnergia,arrayAmplitude] = calcularDados(arrayFinal,arrayDesvio,arrayEnergia,arrayAmplitude)
    for digito = 0:9
        numAudio = 1;
        for i = 1:length(arrayFinal)
            nomeAudio = arrayFinal{i}{1};
            if digito == str2double(nomeAudio(1))
                sinal = arrayFinal{i}{2};
                desvioPadrao = std(sinal);
                arrayDesvio(numAudio, digito + 1) = desvioPadrao;
                energia = sum(abs(sinal).^2);
                arrayEnergia(numAudio, digito + 1) = energia;
                amplitudeMin = min(sinal);
                amplitudeMax = max(sinal);
                razaoAmplitudes = abs(amplitudeMax / amplitudeMin);   
                arrayAmplitude(numAudio, digito + 1) = razaoAmplitudes;
                numAudio = numAudio + 1;
            end
        end
    end
end

function [] = grafico(arrayDados,feature,color)
    figure;
    hold on;
    for i=1:10
        dados = arrayDados(:, i);
        scatter(i-1, dados, color);
    end
    xlabel('Dígito');
    ylabel(feature);
    grid on;
    hold off;
end

function [sinal] = calcularJanela(sinal,tipoJanela)
    amostras = length(sinal);
    if tipoJanela == "hamming"
        janela = hamming(amostras);
    end
    if tipoJanela == "hanning"
        janela = hanning(amostras);
    end
    if tipoJanela == "blackman"
        janela = blackman(amostras);
    end
    sinal = sinal .* janela;
end
 
function [arrayFourierMedian,arrayQuartil25,arrayQuartil75,arrayMax,arrayMin,arrayMedia,arrayfreqEspectral] = fourier(arrayFinal,arrayFourierMedian,arrayQuartil25,arrayQuartil75,arrayMax,arrayMin,arrayMedia,arrayfreqEspectral,tipoJanela)
    espectros = cell(length(arrayFinal),1);
    amostras = 2000;
    audios = 1;
    for digito = 0:9
        numAudio = 1;
        for i = 1:length(arrayFinal)
            nomeAudio = arrayFinal{i}{1};
            if digito == str2double(nomeAudio(1))
                sinal = arrayFinal{i}{2};
                freqAmostra = arrayFinal{i}{3};
                sinal = calcularJanela(sinal,tipoJanela);
                fourier = fft(sinal) / amostras;
                modulo = abs(fourier);
                modulo = modulo(1 : amostras/2);
                modulo(2 : end) = modulo(2 : end ) * 2;
                espectros{audios} = {modulo,digito + 1};
                espectroMax = max(modulo);
                arrayMax(numAudio, digito + 1) = espectroMax;
                espectroMin = min(modulo);
                arrayMin(numAudio, digito + 1) = espectroMin;
                espectroMedia = mean(modulo);
                arrayMedia(numAudio, digito + 1) = espectroMedia;
                energiaTotal = sum(modulo.^2);
                energiaAcumulada = cumsum(modulo.^2);
                indice = find(energiaAcumulada >= 0.90 * energiaTotal);
                frequenciaEspectral = indice(1) / length(modulo);
                arrayfreqEspectral(numAudio, digito + 1) = frequenciaEspectral;
                numAudio = numAudio + 1; 
                audios = audios + 1;
            end
        end
    end
    for digito = 1:10
        for ponto = 1:amostras/2
            espectros_digito = [];
            for audio = 1:length(espectros)
                if espectros{audio}{2} == digito
                    espectros_digito = [espectros_digito, espectros{audio}{1}(ponto)];
                end
            end
            arrayFourierMedian(ponto, digito) = median(espectros_digito);
            arrayQuartil25(ponto,digito) = quantile(espectros_digito, 0.25);
            arrayQuartil75(ponto,digito) = quantile(espectros_digito, 0.75);
        end
    end
    indicePlot = 1;
    figure;
    for i = 1:10
        frequencia = freqAmostra/amostras * (0:amostras/2 - 1);
        subplot(5,2,indicePlot);  
        plot(frequencia,arrayFourierMedian(:,i),'blue');
        hold on;
        plot(frequencia,arrayQuartil25(:,i),'red');
        hold on;
        plot(frequencia,arrayQuartil75(:,i),'yellow');
        hold off;
        title(indicePlot-1);
        indicePlot = indicePlot + 1;
    end  
end

function [arrayMeanSpectr,arrayVarSpectr,arrayMaxSpectr,arrayMedianSpectr] = spectro(arrayFinal,arrayMeanSpectr,arrayVarSpectr,arrayMaxSpectr,arrayMedianSpectr)
    Nwin = round((128e-3)*4800);
    Nsup = round((64e-3)*4800);
    indicePlot = 1;
    figure;
    for i = 5:50:length(arrayFinal)
        sinal = arrayFinal{i}{2};
        subplot(5,2,indicePlot);
        spectrogram(sinal, hamming(Nwin), Nsup, [], 4800, 'yaxis');
        title(indicePlot-1);
        indicePlot = indicePlot + 1;
    end
    for digito = 0:9
        numAudio = 1;
        for i = 1:length(arrayFinal)
            nomeAudio = arrayFinal{i}{1};
            if digito == str2double(nomeAudio(1))
                sinal = arrayFinal{i}{2};
                [s,f,t] = spectrogram(sinal, hamming(Nwin), Nsup, [], 4800, 'yaxis');
                potencia = abs(s).^2;
                potencia_media = mean(potencia, 1);
                media = mean(potencia_media);
                arrayMeanSpectr(numAudio, digito + 1) = media;
                variancia = var(potencia_media);
                arrayVarSpectr(numAudio, digito + 1) = variancia;
                maximo = max(potencia_media);
                arrayMaxSpectr(numAudio, digito + 1) = maximo;
                mediana = median(potencia_media);
                arrayMedianSpectr(numAudio, digito + 1) = mediana;
                numAudio = numAudio + 1;
            end
        end
    end
end

function [digitosContador] = diferenciar(digitosContador,arrayMax,arrayDesvio,arrayfreqEspectral,arrayMedia)
    contador = 0;
    for j = 1:10
        for i = 1:50
            contador = contador + 1;
            if arrayMax(i,j) >= 0.21 %0, 1, 2, 3, 7, 9
                if arrayDesvio(i,j) >= 0.2138 %0, 2, 9
                    if arrayfreqEspectral(i,j) >= 0.140 && arrayfreqEspectral(i,j) <= 0.23 %0
                        digitosContador(contador) = 0;
                    elseif arrayfreqEspectral(i,j) >= 0.24 %9
                        digitosContador(contador) =  9;
                    else %2
                        digitosContador(contador) =  2;
                    end
                else %1, 3, 7
                    if arrayfreqEspectral(i,j) <= 0.208 %1, 3
                        if arrayMedia(i,j) <= 0.0185 && arrayMedia(i,j) >= 0.0093 %1
                            digitosContador(contador) =  1;
                        else %3
                            digitosContador(contador) =  3;
                        end
                    else %7
                        digitosContador(contador) =  7;
                    end
                end
            else %4, 5, 6, 8
                if arrayfreqEspectral(i,j) <= 0.300 %4, 8
                    if arrayfreqEspectral(i,j) >= 0.225 %4
                        digitosContador(contador) =  4;
                    else %8
                        digitosContador(contador) =  8;
                    end
                else %5,6
                    if arrayDesvio(i,j) >= 0.125 %5
                        digitosContador(contador) =  5;
                    else %6
                        digitosContador(contador) =  6;
                    end 
                end
            end
        end
    end
end

function [acertos] = calculo(arrayAudios,digitosContador) % 0
    somatorio = 0;
    fprintf("\n\nDiferenciação:\n\n");
    for i = 1:length(arrayAudios) 
        nome = arrayAudios{i}{1};
        digito = str2double(nome(1));
        fprintf("%d",digito);
    end
    fprintf("\n");
    for i = 1:length(arrayAudios) 
        estimativa = digitosContador(i);
        fprintf("%d",estimativa);
    end
    for i = 1:length(arrayAudios) 
        nome = arrayAudios{i}{1};
        digito = str2double(nome(1));
        estimativa = digitosContador(i);
        if (digito == estimativa)
            somatorio = somatorio + 1;
        end
    end
    acertos = (somatorio / 500) * 100;
end

function [] = grafico3D (arrayAudios,arrayMax, arrayDesvio, arrayfreqEspectral)
    digitos = cell(500,1);
    for i = 1:length(arrayAudios) 
        nome = arrayAudios{i}{1};
        digito = str2double(nome(1));
        digitos{i} = digito;
    end
    digitos = cell2mat(digitos);
    X = arrayMax(:);
    Y = arrayDesvio(:);
    Z = arrayfreqEspectral(:);
    figure;
    scatter3(X, Y, Z, 50, digitos);
    xlabel('Array Max Espectrais');
    ylabel('Array Desvio Padrão');
    zlabel('Array Spectral Edge Freq');
    colorbar;
    title('Melhores Características');
    grid on;
end