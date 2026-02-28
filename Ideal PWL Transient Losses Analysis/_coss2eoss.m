## Copyright 2026 Arief Noor Rahman - Power Control Design
##
## Project  : Ideal PWL Transient Losses Analysis
## Filename : _coss2eoss.cpp
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.

# data format Voltage and Coss
d = [
0,  1200
14.532529082292072, 1150.3401343356827
17.919000430848726, 816.1713022901816
19.396811719086557, 624.9560693979174
22.024989228780655, 430.900447192523
37.03145196036188, 283.3256488947707
54.528220594571266, 223.29910068936366
79.76303317535543, 184.60582151457905
127.08746230073241, 145.56344461648604
191.86126669538987, 120.41583828218435
313.4080137871607, 95.06108711906126
451.7535545023697, 80.24599764714881
609.6510124946144, 71.75028644744775
];
vstep = 240;
nstep = 5;

for i = 1:nstep
  eoss = 0;
  for ii = 1:vstep*i
    for iii = 1:(length(d)-1)
      if (d(iii,1) <= ii)&&(ii <= d(iii+1,1))
        coss = d(iii,2) + ...
        (ii - d(iii,1)) * (d(iii + 1, 2) - d(iii, 2)) / (d(iii + 1, 1) - d(iii, 1));
        break
      endif
    end
    # disp(coss);
    eoss += ii * coss * 1e-12;
  end
  disp([vstep*i eoss]);
end
