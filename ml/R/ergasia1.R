## ---- ask1_nnk
d2 = function(x, y)
{
  return(sqrt(sum((x-y)^2)))
}

nnk = function (x, y, k, x0)
{
  # First some input checking.
  if (!is.matrix(x))
  {
    print("x needs to be a matrix.")
    return(FALSE)
  } else if (!is.vector(y))
  {
    print("y needs to be a vector")
    return(FALSE)
  } else if (!is.numeric(k))
  {
    print("k needs to be a number")
    return(FALSE)
  } else if (length(x[1,]) != length(x0))
  {
    print("X column and x0 length should be the same")
    return(FALSE)
  }
  
  # Now calculate the Nearest Neighbor.
  xdist = apply(x, 1, d2, y=x0)
  dsort = sort(xdist, index.return=TRUE)
  Nk = dsort$ix[1:k]
  f = mean(y[Nk])
  return(f)
}

## ---- ask1_epek
epe = function(y_pred, y_test)
{
  return(sum((y_pred-y_test)^2) / length(y_pred))
}
