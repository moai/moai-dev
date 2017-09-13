# Get Moai Website

This is a proof of concept for the Moai SDK website. The intention is to evaluate GitHub as a possible home for <http://getmoai.com>.

## Contributing to the Site

Feel free to clone this repo and make your changes. Contribute to this site by making a pull request.

## Getting Started

This website uses [GitHub Pages](https://pages.github.com/).

## Tools

So far this site is made using the following tools:

* [Jekyll](http://jekyllrb.com)
* [Bootstrap 3](http://getbootstrap.com)

GitHub Pages automatically support Jekyll (see <https://help.github.com/articles/using-jekyll-with-pages>).

## Getting Started

You don't have to install Jekyll to contribute to the repo, but it would be helpful. If something is not building as expected, installing Jekyll allows you to view the site locally on your computer. The following should get you going -- for more information, see GitHub's [Using Jekyll with Pages](https://help.github.com/articles/using-jekyll-with-pages/) article.

### Install Ruby

First you will need to install Ruby on your system.

* If you are running Mac OS X, no need - it's already installed.
* If you're on Windows, you'll find an installer on <http://rubyinstaller.org/downloads>. Also install the appropriate *Development kit* (which you'll find on the same website). Make sure to add the Ruby and DevKit executables to your PATH.

### Install the Bundler gem

Once you have Ruby, you will need to install the [Bundler gem](http://bundler.io/) by running `gem install bundler`.

### Install Jekyll

GitHub provides a [Ruby gem](https://github.com/github/pages-gem) that automatically sets up [Jekyll](http://jekyllrb.com/) in a way that's compatible with GitHub Pages. To install it, navigate into the root directory of your repo and run `bundle install`. 

### Run

Once everything is installed, you can run the following command:

`bundle exec jekyll serve`

This will tell Jekyll to compile your the site and serve it up locally on port 4000: <http://localhost:4000/moai-sdk/>.

As of version 2.4, the `serve` command will watch for changes automatically.