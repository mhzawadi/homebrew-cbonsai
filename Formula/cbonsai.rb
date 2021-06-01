class Cbonsai < Formula
  desc "Console Bonsai is a bonsai tree generator, written in C using ncurses"
  homepage "https://github.com/mhzawadi/homebrew-cbonsai"
  url "https://github.com/mhzawadi/homebrew-cbonsai/archive/v1.2.0.tar.gz"
  sha256 "7dba90fa9a922aa6e750f92ef704178bb3fb52c06752dac34a3718eb4554d2e1"
  license "GPL-3.0"

  def install
    system "make"
  end

  test do
    system "~/.local/bin/cbonsai", "-p"
  end
end
