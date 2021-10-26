source('../R/ergasia1.R')

library(testthat)

# Set some dummy data.
X = rbind(
  c(0, 0), c(1, 1), c(3, 4), c(4, 5)
)
y = c(4, 2, 11, 1)
Ey = mean(y)

test_that("Euclidian distance", {
  expect_equal(d2(9, 1), 8)
  expect_equal(d2(c(1, 1), c(4, 5)), 5)
})

test_that("Nearest Neighbour (k)", {
  expect_equal(nnk(X, y, 1, c(0, 0)), 4)
  expect_equal(nnk(X, y, 1, c(3, 3)), 11)
  expect_equal(nnk(X, y, 2, c(1.5, 1.5)), 3)
  expect_equal(nnk(X, y, 4, c(2,2)), Ey)
  expect_equal(nnk(X, y, 4, c(4,5)), Ey)
})

test_that("Expected Prediction Error", {
  expect_equal(epe(y, y), 0)
  expect_equal(epe(y, c(4, 0, 11, 1)), 1)
})
