claims = scan("claims.txt")

mu = 4
n = length(claims)
sample_variance = var(claims)
sample_mean = mean(claims)
t_statistic = n ** .5 * (sample_mean - mu) / sample_variance

# if the data was normally distributed, the t-test statistic would also be normally distributed

p = min(pnorm(t_statistic), 1 - pnorm(t_statistic))


samples = 10000
vector = c()
for (val in 1: samples) {
  x = rgamma(n=10, shape=2, rate=0.5)
  x_mean = mean(x)
  x_var = var(x)
  x_n = length(x)
  x_t = x_n ** .5 * (x_mean - mu) / x_var
  vector = c(vector, x_t)
}

desired_p = 0.05

lborder = as.integer(samples * desired_p / 2)
rborder = samples - lborder

sorted_vector = sort(vector)

hist(sorted_vector, nclass=100)
sorted_vector[rborder]

rejected = t_statistic < sorted_vector[lborder] || t_statistic > sorted_vector[rborder]
print(t_statistic)
print(sorted_vector[lborder])
print(sorted_vector[rborder])