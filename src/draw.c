#include "../fractol.h"

void		put_pxl(t_fractol *fr, int x, int y, unsigned int c)
{
	int		i;

	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return ;
	i = (x * 4) + (y * fr->s_line);
	fr->pxl[i] = c;
	fr->pxl[++i] = c >> 8;
	fr->pxl[++i] = c >> 16;
}

void		*drawthr(void *fract)
{
	int			x, y;	
	t_fractol	*fr;

	fr = (t_fractol *)fract;
	x = 0;
	while (x < WIDTH)
	{
		y = fr->parth;
		while (y <= fr->parth + fr->hstep)
		{
			fr->fun(fr, x, y);
			y++;
		}
		x++;
	}
	pthread_exit(0);
	return (NULL);
}

void		get_threads(t_fractol *fr)
{
	pthread_t		thr[THREADS];
	t_fractol		frx[THREADS];
	int				i;
	int				ret;
	int				y;

	fr->centerx = (3 * HEIGHT / (fr->zoom * WIDTH));
	fr->centery = 2 / fr->zoom;
	i = 0;
	y = 0;
	while (i < THREADS)
	{	
		frx[i] = *fr;
		frx[i].parth = y;
		y += fr->hstep;
		if ((ret = pthread_create(&thr[i], NULL, drawthr, (void *)&frx[i])))
			exiterror(ft_strjoin(PCREA_ERR, ft_itoa(ret)), fr);
		i++;
	}
	while (i-- > 0)
		if ((ret = pthread_join(thr[i], NULL)))
			exiterror(ft_strjoin(PJOIN_ERR, ft_itoa(ret)), fr);
	mlx_put_image_to_window(fr->mlx, fr->win, fr->img, 0, 0);
	mlx_string_put(fr->mlx, fr->win, 20, 20, WHITE, ft_itoa(fr->iter));
}
