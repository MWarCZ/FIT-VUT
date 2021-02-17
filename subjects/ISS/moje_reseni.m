% xvalka05
% MWarCZ -/\/\-\/\/-
%
figure_index = 1;
% 1
% Nacte zvuk
[y,fs] = audioread('xvalka05.wav');

fs % Vypis vzorkovaci frekvenci
pocet_vzorku = length(y) % Delka signalu/pocet vzorku
delka_v_sekundach = length(y)/fs % Delka v sekundach

% 2
y_fft = abs(fft(y)); % diskretni Fourierova transformace
osa_f = (0 : ( pocet_vzorku / 2 - 1) ) / pocet_vzorku * fs; % Vytvoreni frekvenci osy - meritko
y_fft = y_fft(1 : pocet_vzorku / 2);
figure(figure_index);
figure_index=figure_index+1;
plot(osa_f, y_fft);
title('Spektrum signalu z xvalka05.wav.');
xlabel('f [Hz]');

% 3
% Maximum spektra na frekvenci
max_spektra_na_f = ( find(max(y_fft) == y_fft, 1, 'first') - 1 ) * fs / length(y_fft) / 2

% 4
filtr_b = [0.2324, -0.4112, 0.2324];
filtr_a = [1.0, 0.2289, 0.4662];
figure(figure_index);
figure_index=figure_index+1;
ukazmito(filtr_b, filtr_a, fs);

%5
h = abs( freqz(filtr_b, filtr_a, 256) );
f = (0:255) / 256 * fs / 2;
figure(figure_index);
figure_index=figure_index+1;
plot(f, h);
xlabel('f');
ylabel('|H(f)|');
title('Kmitoctova charakteristika filtru.');

%6
y_filtr = filter(filtr_b, filtr_a, y);
y_filtr_fft = abs( fft(y_filtr) );
osa_f = (0:pocet_vzorku/2-1)/pocet_vzorku*fs;
y_filtr_fft = y_filtr_fft(1: pocet_vzorku/2);
figure(figure_index);
figure_index=figure_index+1;
plot(osa_f, y_filtr_fft);
title('Spektrum signalu z xvalka05.wav po projeti filtrem.');
xlabel('f [Hz]');

%7
max_spektra_filtrovaneho_signalu_na_f = ( find(max(y_filtr_fft) == y_filtr_fft, 1, 'first') - 1 ) * fs / length(y_filtr_fft) / 2

% ---- Dale pracovat s puvodnim signalem -----
%8

%bb = 1.0;
%filtr_bb = [bb, bb, -bb , -bb];
%filtr_aa = [ 1.0 ];
%y_filtr = filter(filtr_bb, filtr_aa, y);
%figure(figure_index);
%figure_index=figure_index+1;
%plot(y_filtr);
%title('Hledany signal.');

%----------

%y_fft = abs(fft(y)); % diskretni Fourierova transformace
%osa_f = (0 : ( pocet_vzorku / 2 - 1) ) / pocet_vzorku * fs; % Vytvoreni frekvenci osy - meritko
%y_fft = y_fft(1 : pocet_vzorku / 2);
%figure(figure_index);
%figure_index=figure_index+1;
%plot(osa_f, y_fft);
%title('Spektrum signalu z xvalka05.wav.');
%xlabel('Hz');
%----------

%hz = fs/4e3
%for ii=1:(pocet_vzorku-4),
%	if(y(ii)==-y(ii+hz))
%		disp(['Shoda na vzorku: ']); 
%		ii
%		y(ii)
%		y(ii+hz)
%		y(ii+2*hz)
%		y(ii+3*hz)
%	end
%end
%----------

%Fn = Fs/2; % Nyquist Frequency (Hz)
%Wp = [0.503 0.523]/Fn;% Passband Frequency (Normalised)
%Ws = [0.483 0.543]/Fn;% Stopband Frequency (Normalised)
%Rp =   1;% Passband Ripple (dB)
%s =  50;% Stopband Ripple (dB)
%[n,Ws] = cheb1ord(Wp,Ws,Rp,Rs);% Filter Order
%[z,p,k] = cheby1(n,Rs,Ws,'stop');% Filter Design
%[sossb,gsb] = zp2sos(z,p,k);% Convert To Second-Order-Section For Stability
%----------
%afiltr_b = [0.2324, -0.4112, 0.2324];
%afiltr_a = [1.0, 0.2289, 0.4662];
%figure(figure_index);

%tmp_y = filter(afiltr_b,afiltr_a,y);
%figure(figure_index);
%figure_index=figure_index+1;
%plot(tmp_y);

%bfiltr_b = [0.2324, 0.4112, 0.2324];
%bfiltr_a = [1.0, 0.2, 0.5];

%tmp_y = filter(afiltr_b,afiltr_a,tmp_y);
%figure(figure_index);
%figure_index=figure_index+1;
%plot(tmp_y);

%--------------------------------------------------------
% Nenasel jsem ani po 2 dnech - vzdávám se vzhráli jste.
%--------------------------------------------------------

%9
% Prepsani vzorecku 
R = zeros(1,100);
for k = -50:50,
	tmp = 0;
	for n = 1:pocet_vzorku,
		if (k + n > 0) && (k + n < pocet_vzorku)
			tmp = tmp + y(n) * y(n + k);
		end
	end
	R(k + 50 + 1) = 1/pocet_vzorku * tmp;
end
figure(figure_index);
figure_index=figure_index+1;
stem(-50:50, R); 
xlabel('k'); 
ylabel('R[k]');
%10
R10 = R(10)

% ----- odhad sdruzene funkce hustoty roydeleni provdepodobnosti -----
%11

% matrix_y = zeros(pocet_vzorku, 100);
% for ii=1:pocet_vzorku,
%   matrix_y(ii,:) = y(ii); 
% end

% xmin = min(min(matrix_y));
% xmax = max(max(matrix_y));
% kolik = 50;
% x = linspace(xmin,xmax,kolik);

% figure(figure_index);
% figure_index=figure_index+1;

% n1=10;
% n2=n1;
% 	[h,p,r] = hist2opt(matrix_y(:,n1),matrix_y(:,n2),x);
% 	imagesc(x,x,p); 
% 	axis xy; 
% 	colorbar; 
% 	xlabel ('x2'); 
% 	ylabel('x1');
% 	[n1 n2 r]

% %---------------------------

%aux = ksid(1:10,:)'; 
%ksi = aux(:); 
ksi = y;
%X1 = 0:36; 
X1 = linspace(min(y),max(y),50);
X2 = X1; 
N = length(ksi); 

k=0; 
data1 = ksi(1:end-k); 
data2 = ksi(k+1:end); 

P = hist3([data1 data2], {X1 X2}) / N; 
imagesc(X1, X2, P); colorbar; title('P(X_1,X_2,n_1,n_2)');axis xy; 
print -dpng ergo_2d_roul_k_0.png

kontrola = sum(sum(P)) 

k=10; 
data1 = ksi(1:end-k); 
data2 = ksi(k+1:end); 

P = hist3([data1 data2], {X1 X2}) / N; 
imagesc(X1, X2, P); colorbar; title('P(X_1,X_2,n_1,n_2)');axis xy; 
print -dpng ergo_2d_roul_k_10.png

kontrola = sum(sum(P)) 

%12
kontrola = sum(sum(P)) 

%13
x = X1;
L = length(x);
x = x(:); X1 = repmat(x,1,L);
x = x'; X2 = repmat(x,L,1); 
r = sum(sum (X1 .* X2 .* P))



