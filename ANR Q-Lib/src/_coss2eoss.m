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
0.1275983846353057, 6.529714476876379e-9
2.635931396585258, 4.434340598604771e-9
6.402232420788181, 3.2221290960553184e-9
9.164605221837174, 2.6238046733390025e-9
9.91118799825902, 1.555909495268473e-9
11.08205193188142, 1.3266560775675627e-9
14.346223561625724, 1.0089708941347145e-9
20.877872478747374, 7.323825685215609e-10
26.90838369870033, 5.956475973897536e-10
33.77919308897178, 5.677226275479561e-10
60.09090558490858, 5.250802293028064e-10

];
vstep = 12;
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
    eoss += ii * coss * 1;
  end
  disp([vstep*i eoss]);
end
